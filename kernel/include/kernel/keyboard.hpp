#ifndef __KEYBOARD_H
#define __KEYBOARD_H

#include <types.h>
#include <kernel/interrupts.hpp>
#include <kernel/port.hpp>

class KeyboardDriver : public InterruptHandler
{
    Port8Bit dataport;
    Port8Bit commandport;

public:
    KeyboardDriver(InterruptManager *manager);
    virtual uint32_t HandleInterrupt(uint32_t esp);
};

#endif