#ifndef __MYOS__MEMORY_MANAGEMENT
#define __MYOS__MEMORY_MANAGEMENT

#include <types.h>

struct MemoryChunk
{
    MemoryChunk *next;
    MemoryChunk *prev;
    bool allocated;
    size_t size;
};

class MemoryManager
{
protected:
    MemoryChunk *first;

public:
    static MemoryManager *activeMemoryManager;

    MemoryManager(size_t first, size_t size);

    void *malloc(size_t size);
    void free(void *ptr);
};

void *operator new(size_t size);
void *operator new[](size_t size);

void *operator new(size_t size, void *ptr);
void *operator new[](size_t size, void *ptr);

void operator delete(void *ptr);
void operator delete[](void *ptr);

#endif