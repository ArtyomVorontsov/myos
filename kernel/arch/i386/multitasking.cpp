#include <kernel/multitasking.hpp>
#include <stdio.h>
#include <types.h>

Task::Task(GlobalDescriptorTable *gdt, void entryPoint())
{
    cpuState = (CPUState *)(stack + 4096 - sizeof(CPUState));

    cpuState->eax = 0;
    cpuState->ebx = 0;
    cpuState->ecx = 0;
    cpuState->edx = 0;

    cpuState->esi = 0;
    cpuState->edi = 0;
    cpuState->ebp = 0;

    cpuState->eip = (uint32_t)entryPoint;
    cpuState->cs = gdt->CodeSegmentSelector();
    cpuState->eflags = 0x202;
};

TaskManager::TaskManager()
{
    numTasks = 0;
    currentTask = -1;
};

bool TaskManager::addTask(Task *task)
{
    if (numTasks >= 256)
    {
        return false;
    }

    tasks[numTasks++] = task;

    return true;
}

CPUState *TaskManager::schedule(CPUState *cpuState)
{
    if (numTasks <= 0)
    {
        return cpuState;
    }

    if (currentTask >= 0)
    {
        tasks[currentTask]->cpuState = cpuState;
    }

    if (++currentTask >= numTasks)
    {
        currentTask = currentTask %= numTasks;
    }

    return tasks[currentTask]->cpuState;
}

void TaskManager::printTaskState()
{
    printf("Print task state\n");
    for (uint8_t i = 0; i < numTasks; i++)
    {
        printf("eip %l\n", tasks[i]->cpuState->eip);
        printf("cs %l\n", tasks[i]->cpuState->cs);
        printf("esp %l\n", tasks[i]->cpuState);
    }
}