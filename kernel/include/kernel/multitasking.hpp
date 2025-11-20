#ifndef __MYOS__MULTITASKING_H
#define __MYOS__MULTITASKING_H

#include <types.h>
#include <kernel/gdt.hpp>

struct CPUState
{
    uint32_t eax;
    uint32_t ebx;
    uint32_t ecx;
    uint32_t edx;

    uint32_t esi;
    uint32_t edi;
    uint32_t ebp;

    uint32_t error;

    uint32_t eip;
    uint32_t cs;
    uint32_t eflags;
    uint32_t esp;
    uint32_t ss;
} __attribute__((packed));

class Task
{
    friend class TaskManager;

private:
    uint8_t stack[4096];

    CPUState *cpuState;

public:
    Task(GlobalDescriptorTable *gdt, void entryPoint());
};

class TaskManager
{
private:
    Task *tasks[256];
    int numTasks;
    int currentTask;

public:
    TaskManager();
    void printTaskState();

    bool addTask(Task *task);
    CPUState *schedule(CPUState *cpuState);
};

#endif
