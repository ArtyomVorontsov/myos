#ifndef __MYOS__HARDWARECOMMUNICATION__PCI_H
#define __MYOS__HARDWARECOMMUNICATION__PCI_H

#include <hardwarecommunication/port.hpp>
#include <hardwarecommunication/bar.hpp>
#include <driver/driver.hpp>
#include <driver/driver-manager.hpp>
#include <types.h>
#include <kernel/interrupts.hpp>

class PeripheralComponentInterconnectDeviceDescriptor
{
public:
    uint32_t portBase;
    uint32_t interrupt;

    uint16_t bus;
    uint16_t device;
    uint16_t function;
    uint16_t vendor_id;
    uint16_t device_id;
    uint8_t class_id;
    uint8_t subclass_id;
    uint8_t interface_id;
    uint8_t revision_id;

    PeripheralComponentInterconnectDeviceDescriptor();
};

class PeripheralComponentInterconnectController
{
    Port32Bit dataPort;
    Port32Bit commandPort;

public:
    PeripheralComponentInterconnectController();

    uint32_t Read(
        uint16_t bus,
        uint16_t device,
        uint16_t function,
        uint32_t registerOffset);

    void Write(
        uint16_t bus,
        uint16_t device,
        uint16_t function,
        uint32_t registerOffset,
        uint32_t value);

    bool DeviceHasFunctions(uint16_t bus, uint16_t device);

    void SelectDrivers(DriverManager *driverManager, InterruptManager *interruptManager);
    Driver *GetDriver(PeripheralComponentInterconnectDeviceDescriptor dev, InterruptManager *interruptManager);
    BaseAddressRegister GetBaseAddressRegister(uint16_t bus, uint16_t device, uint16_t function, uint16_t barNumber);

    PeripheralComponentInterconnectDeviceDescriptor GetDeviceDescriptor(
        uint16_t bus,
        uint16_t device,
        uint16_t function);
};

#endif