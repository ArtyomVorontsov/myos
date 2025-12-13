
#ifndef __MYOS__SYSCALLS_H
#define __MYOS__SYSCALLS_H

#include <types.h>
#include <kernel/interrupts.hpp>
#include <kernel/multitasking.hpp>

class SyscallHandler : public InterruptHandler
{

public:
    SyscallHandler(InterruptManager *interruptManager, uint8_t InterruptNumber);

    virtual uint32_t HandleInterrupt(uint32_t esp);
};

#endif