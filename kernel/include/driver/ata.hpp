#ifndef __MYOS__DRIVERS__ATA_H
#define __MYOS__DRIVERS__ATA_H

#include <types.h>
#include <kernel/interrupts.hpp>
#include <hardwarecommunication/port.hpp>

class AdvancedTechnologyAttachment
{
protected:
    bool master;

    Port16Bit dataPort;
    Port8Bit errorPort;
    Port8Bit sectorCountPort;
    Port8Bit lbaLowPort;
    Port8Bit lbaMidPort;
    Port8Bit lbaHiPort;
    Port8Bit devicePort;
    Port8Bit commandPort;
    Port8Bit controlPort;

public:
    AdvancedTechnologyAttachment(bool master, uint16_t portBase);

    void Identify();
    void Read28(uint32_t sectorNum, uint8_t* data, int count = 512);
    void Write28(uint32_t sectorNum, uint8_t *data, uint32_t count);
    void Flush();
};

#endif