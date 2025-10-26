#ifndef __MOUSE_H
#define __MOUSE_H

#include <types.h>
#include <kernel/port.hpp>
#include <kernel/interrupts.hpp>

class MouseDriver : public InterruptHandler
{
    Port8Bit dataport;
    Port8Bit commandport;
    uint8_t buffer[3];

    uint8_t offset;
    uint8_t buttons;

    int8_t x, y;

public:
    MouseDriver(InterruptManager *manager);
    virtual uint32_t HandleInterrupt(uint32_t esp);
};

#endif