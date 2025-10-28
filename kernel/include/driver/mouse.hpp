#ifndef __MOUSE_H
#define __MOUSE_H

#include <types.h>
#include <kernel/port.hpp>
#include <kernel/interrupts.hpp>
#include <driver/driver.hpp>
#include <event-handler/mouse-event-handler.hpp>

class MouseDriver : public InterruptHandler,
                    public Driver
{
    Port8Bit dataport;
    Port8Bit commandport;
    uint8_t buffer[3];

    uint8_t offset;
    uint8_t buttons;

    MouseEventHandler *handler;

public:
    MouseDriver(
        InterruptManager *manager,
        MouseEventHandler *handler);
    virtual uint32_t HandleInterrupt(uint32_t esp);
    virtual void Activate();
};

#endif