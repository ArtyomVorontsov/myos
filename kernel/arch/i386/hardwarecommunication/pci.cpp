

#include <hardwarecommunication/port.hpp>
#include <driver/driver.hpp>
#include <driver/driver-manager.hpp>
#include <types.h>
#include <kernel/interrupts.hpp>
#include <hardwarecommunication/pci.hpp>
#include <stdio.h>

PeripheralComponentInterconnectDeviceDescriptor::PeripheralComponentInterconnectDeviceDescriptor()
{
}

PeripheralComponentInterconnectController::PeripheralComponentInterconnectController()
    : dataPort(0xCFC),
      commandPort(0xCF8)
{
}

uint32_t PeripheralComponentInterconnectController::Read(
    uint16_t bus,
    uint16_t device,
    uint16_t function,
    uint32_t registerOffset)
{

    uint32_t id = 0x1 << 31 | ((bus & 0xFF) << 16) | ((device & 0x1F) << 11) | ((function & 0x07) << 8) | (registerOffset & 0xFC);

    commandPort.Write(id);
    uint32_t result = dataPort.Read();

    return result >> (8 * (registerOffset % 4));
}

void PeripheralComponentInterconnectController::Write(
    uint16_t bus,
    uint16_t device,
    uint16_t function,
    uint32_t registerOffset,
    uint32_t value)
{

    uint32_t id = 0x1 << 31 | ((bus & 0xFF) << 16) | ((device & 0x1F) << 11) | ((function & 0x07) << 8) | (registerOffset & 0xFC);
    commandPort.Write(id);
    dataPort.Write(value);
}

bool PeripheralComponentInterconnectController::DeviceHasFunctions(uint16_t bus, uint16_t device)
{
    return Read(bus, device, 0, 0x0E) & (1 << 7);
}

void PeripheralComponentInterconnectController::SelectDrivers(DriverManager *driverManager, InterruptManager *interruptManager)
{
    for (uint8_t bus = 0; bus < 8; bus++)
    {
        for (uint8_t device = 0; device < 32; device++)
        {

            uint8_t numFunctions = DeviceHasFunctions(bus, device) ? 8 : 1;

            for (uint8_t function = 0; function < numFunctions; function++)
            {
                PeripheralComponentInterconnectDeviceDescriptor dev = GetDeviceDescriptor(bus, device, function);

                if (dev.vendor_id == 0x000 || dev.vendor_id == 0xFFFF)
                {
                    continue;
                }

                for (int barNumber = 0; barNumber < 6; barNumber++)
                {
                    BaseAddressRegister bar = GetBaseAddressRegister(bus, device, function, barNumber);

                    if (bar.address && (bar.type == InputOutput))
                    {
                        dev.portBase = (uint32_t)bar.address;
                    }

                    Driver *driver = GetDriver(dev, interruptManager);

                    if (driver != 0)
                    {
                        driverManager->AddDriver(driver);
                    }
                }

                printf("PCI BUS ");

                printfHex(bus & 0xFF);

                printf(", DEVICE ");

                printfHex(device & 0xFF);

                printf(", FUNCTION ");

                printfHex(function & 0xFF);

                printf(" = VENDOR ");

                printfHex((dev.vendor_id & 0xFF00) >> 8);

                printfHex(dev.vendor_id & 0xFF);

                printf(", DEVICE ");

                printfHex((dev.device_id & 0xFF00) >> 8);

                printfHex(dev.device_id & 0xFF);

                printf("\n");
            }
        }
    }
}

BaseAddressRegister PeripheralComponentInterconnectController::GetBaseAddressRegister(
    uint16_t bus,
    uint16_t device,
    uint16_t function,
    uint16_t barNumber)
{
    BaseAddressRegister result;

    uint32_t headerType = Read(bus, device, function, 0x0E) & 0x7F;
    int maxBARs = 6 - (4 * headerType);

    if (barNumber >= maxBARs)
    {
        return result;
    }

    uint32_t barValue = Read(bus, device, function, 0x10 + 4 * barNumber);
    result.type = (barValue & 0x01) ? InputOutput : MemoryMapping;

    // uint32_t temp;

    if (result.type == MemoryMapping)
    {
        switch ((barValue >> 1) & 0x3)
        {
        case 0: // 32 Bit Mode
        case 1: // 20 Bit Mode
        case 2: // 64 Bit Mode
            break;
        }
    }
    else // InputOutput
    {
        result.address = (uint8_t *)(barValue & ~0x3);
        result.prefetchable = false;
    }

    return result;
}

Driver *PeripheralComponentInterconnectController::GetDriver(PeripheralComponentInterconnectDeviceDescriptor dev, InterruptManager *interruptManager)
{
    switch (dev.vendor_id)
    {
    case 0x1022: // AMD
        printf("AMD Device, ");
        switch (dev.device_id)
        {
        case 0x2000: // am79c973
            printf("AMD am79c973, ");
            break;

        default:
            break;
        }

        break;

    case 0x8086: // Intel
        printf("Intel Device, ");

        switch (dev.device_id)
        {
        case 0x1237:
            printf("Intel PCI & Memory, ");
            break;

            case 0x7000:
            printf("Intel TBC, ");
            break;

            case 0x7010:
            printf("Intel Ethernet/MAC, ");
            break;

            case 0x7113:
            printf("Intel UNKNOWN DEVICE, ");
            break;

            case 0x100E:
            printf("Intel Ethernet i217, ");
            break;

        default:
            break;
        }

        break;

    default:
        break;
    }

    return 0;
}

PeripheralComponentInterconnectDeviceDescriptor PeripheralComponentInterconnectController::GetDeviceDescriptor(
    uint16_t bus,
    uint16_t device,
    uint16_t function)
{
    PeripheralComponentInterconnectDeviceDescriptor result;

    result.bus = bus;
    result.device = device;
    result.function = function;

    result.vendor_id = Read(bus, device, function, 0x00);
    result.device_id = Read(bus, device, function, 0x02);

    result.class_id = Read(bus, device, function, 0x0b);
    result.subclass_id = Read(bus, device, function, 0x0a);
    result.interface_id = Read(bus, device, function, 0x09);

    result.revision_id = Read(bus, device, function, 0x08);
    result.interrupt = Read(bus, device, function, 0x3c);

    return result;
}
