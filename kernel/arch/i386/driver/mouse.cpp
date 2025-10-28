#include <driver/mouse.hpp>

MouseDriver::MouseDriver(
    InterruptManager *interruptManager,
    MouseEventHandler *handler)
    : InterruptHandler(interruptManager, 0x2C),
      dataport(0x60),
      commandport(0x64)
{
    this->handler = handler;
};

void MouseDriver::Activate()
{

    offset = 0;
    buttons = 0;

    commandport.Write(0xA8);
    commandport.Write(0x20);

    uint8_t status = dataport.Read() | 2;

    commandport.Write(0x60);

    dataport.Write(status);

    commandport.Write(0xD4);
    dataport.Write(0xF4);
    dataport.Read();

    if (handler != 0)
    {
        handler->OnActivate();
    }
};

uint32_t MouseDriver::HandleInterrupt(uint32_t esp)
{

    if (handler == 0)
    {
        return esp;
    }

    uint8_t status = commandport.Read();

    if (!(status & 0x20))
        return esp;

    buffer[offset] = dataport.Read();

    offset = (offset + 1) % 3;

    if (offset == 0)
    {
        if (buffer[1] != 0 || buffer[2] != 0)
        {
            handler->OnMouseMove(buffer[1], -buffer[2]);
        }
    }

    return esp;
};