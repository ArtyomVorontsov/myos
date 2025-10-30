#ifndef __MY_OS_HARDWARECOMMUNICATION_BAR
#define __MY_OS_HARDWARECOMMUNICATION_BAR
#include <types.h>

enum BaseAddressRegisterType
{
    MemoryMapping = 0,
    InputOutput = 1
};

class BaseAddressRegister
{
public:
    bool prefetchable;
    uint8_t *address;
    uint32_t *size;
    BaseAddressRegisterType type;
};

#endif