#ifndef __KEYBOARD_H
#define __KEYBOARD_H

#include <types.h>
#include <kernel/interrupts.hpp>
#include <kernel/port.hpp>
#include <driver/driver.hpp>
#include <event-handler/keyboard-event-handler.hpp>


class KeyboardDriver : public InterruptHandler,
                       public Driver
{
    Port8Bit dataport;
    Port8Bit commandport;

    KeyboardEventHandler *handler;

public:
    KeyboardDriver(InterruptManager *manager, KeyboardEventHandler *handler);
    virtual uint32_t HandleInterrupt(uint32_t esp);
    virtual void Activate();
};

#endif