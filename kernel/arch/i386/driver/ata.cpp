#include <driver/ata.hpp>
#include <stdio.h>

// LOG CONFIG
// #define LOG_ATA true

AdvancedTechnologyAttachment::AdvancedTechnologyAttachment(bool master, uint16_t portBase)
    : dataPort(portBase),
      errorPort(portBase + 0x1),
      sectorCountPort(portBase + 0x2),
      lbaLowPort(portBase + 0x3),
      lbaMidPort(portBase + 0x4),
      lbaHiPort(portBase + 0x5),
      devicePort(portBase + 0x6),
      commandPort(portBase + 0x7),
      controlPort(portBase + 0x206)
{
    this->master = master;
}

void AdvancedTechnologyAttachment::Identify()
{
    devicePort.Write(master ? 0xA0 : 0xB0);
    controlPort.Write(0);

    devicePort.Write(0xA0);
    uint8_t status = commandPort.Read();
    if (status == 0xFF)
        return;

    devicePort.Write(master ? 0xA0 : 0xB0);
    sectorCountPort.Write(0);
    lbaLowPort.Write(0);
    lbaMidPort.Write(0);
    lbaHiPort.Write(0);
    commandPort.Write(0xEC); // identify command

    // check if device exist
    status = commandPort.Read();
    if (status == 0x00)
        return;

    while (((status & 0x80) == 0x80) &&
           ((status & 0x01) != 0x01))
        status = commandPort.Read();

    if (status & 0x01)
    {
        printf("ERROR");
        return;
    }

    for (int i = 0; i < 256; i++)
    {
        uint16_t data = dataPort.Read();
        char text[4] = "  \0";
        text[0] = (data >> 8) & 0xFF;
        text[1] = data & 0xFF;

#ifdef LOG_ATA
        printf(text);
#endif
    }

#ifdef LOG_ATA
    printf("\n");
#endif
}

void AdvancedTechnologyAttachment::Read28(uint32_t sectorNum, uint8_t *data, int count)
{
    if (sectorNum > 0x0FFFFFFF)
        return;

    // We are in 28bit mode and LBA has 24 bits for the address, so there are 4bits left
    // So we put them in the device port
    devicePort.Write((master ? 0xE0 : 0xF0) |
                     ((sectorNum & 0x0F000000) >> 24));

    errorPort.Write(0);

    // we will read or write on a single sector
    sectorCountPort.Write(1);
    lbaLowPort.Write(sectorNum & 0x000000FF);
    lbaMidPort.Write((sectorNum & 0x0000FF00) >> 8);
    lbaHiPort.Write((sectorNum & 0x00FF0000) >> 16);
    // Read command
    commandPort.Write(0x20);

    uint8_t status = commandPort.Read();

    while (((status & 0x80) == 0x80) &&
           ((status & 0x01) != 0x01))
        status = commandPort.Read();

    if (status & 0x01)
    {
        printf("ERROR");
        return;
    }

#ifdef LOG_ATA
    printf("Reading ATA Drive: ");
#endif

    for (int i = 0; i < count; i += 2)
    {
        uint16_t wdata = dataPort.Read();

        // char text[4] = "  \0";
        // text[0] = wdata & 0xFF;

        // if (i + 1 < count)
        //     text[1] = (wdata >> 8) & 0xFF;
        // else
        //     text[1] = '\0';

        // printf(text);

        data[i] = wdata & 0x00FF;
        if (i + 1 < count)
            data[i + 1] = (wdata >> 8) & 0x00FF;
    }

    for (int i = count + (count % 2); i < 512; i += 2)
    {
        dataPort.Read();
    }

#ifdef LOG_ATA
    printf("\n");
#endif LOG_ATA
}

void AdvancedTechnologyAttachment::Write28(uint32_t sectorNum, uint8_t *data, uint32_t count)
{
    if (sectorNum > 0x0FFFFFFF)
        return;

    // refuse writing more than a sector
    if (count > 512)
        return;

    // We are in 28bit mode and LBA has 24 bits for the address, so there are 4bits left
    // So we put them in the device port
    devicePort.Write((master ? 0xE0 : 0xF0) |
                     ((sectorNum & 0x0F000000) >> 24));

    errorPort.Write(0);

    // we will read or write on a single sector
    sectorCountPort.Write(1);
    lbaLowPort.Write(sectorNum & 0x000000FF);
    lbaMidPort.Write((sectorNum & 0x0000FF00) >> 8);
    lbaHiPort.Write((sectorNum & 0x00FF0000) >> 16);
    // Write command
    commandPort.Write(0x30);

#ifdef LOG_ATA
    printf("Writing to ATA drive: ");
#endif
    for (int i = 0; i < count; i += 2)
    {

        uint16_t wdata = data[i];
        if (i + 1 < count)
            // if the next byte is available we write it also
            wdata |= ((uint16_t)data[i + 1]) << 8;

        dataPort.Write(wdata);

        char text[4] = "  \0";
        text[1] = (wdata >> 8) & 0xFF;
        text[0] = wdata & 0xFF;

#ifdef LOG_ATA
        printf(text);
#endif
    }

    // If we write less than full size of the sector - we write zeroes to the rest of the sector
    // and if count is an odd number then the first byte must have been written in the wdata loop as zero
    // so we cover this case and write the second byte of the 2 bytes only as zero also
    for (int i = count + (count % 2); i < 512; i += 2)
    {
        dataPort.Write(0x0000);
    }

#ifdef LOG_ATA
    printf("\n");
#endif
}

void AdvancedTechnologyAttachment::Flush()
{
    devicePort.Write(master ? 0xE0 : 0xF0);
    commandPort.Write(0xE7);

    uint8_t status = commandPort.Read();
    if (status == 0x00)
        return;

    while (((status & 0x80) == 0x80) &&
           ((status & 0x01) != 0x01))
        status = commandPort.Read();

    if (status & 0x01)
    {
        printf("ERROR");
        return;
    }
}
