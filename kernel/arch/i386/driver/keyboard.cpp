#include <driver/keyboard.hpp>
#include <stdio.h>

KeyboardDriver::KeyboardDriver(
    InterruptManager *manager,
    KeyboardEventHandler *handler)
    : InterruptHandler(manager, 0x21),
      dataport(0x60),
      commandport(0x64)
{

    this->handler = handler;
    this->bufferCurrentIndex = 0;
    this->bufferStartIndex = 0;
    this->bufferEndIndex = 127;
    this->bufferPendingAmount = 0;
};

void KeyboardDriver::Activate()
{

    while (commandport.Read() & 0x1)
    {
        dataport.Read();
    }
    commandport.Write(0xae); // Activate interrupts
    commandport.Write(0x20); // Command 0x20 = Read contoller command byte
    uint8_t status = (dataport.Read() | 1) & ~0x10;
    commandport.Write(0x60); // Command 0x60 = set controller command byte
    dataport.Write(status);
    dataport.Write(0xf4);

    if (handler != 0)
    {
        handler->OnActivate();
    }
}

char KeyboardDriver::getLastBufferChar()
{
    return this->buffer[this->bufferCurrentIndex - 1];
}

void KeyboardDriver::bufferPush(char c)
{
    buffer[this->bufferCurrentIndex] = c;

    this->bufferCurrentIndex++;
    this->bufferPendingAmount++;
    if (this->bufferEndIndex < this->bufferCurrentIndex)
    {
        this->bufferCurrentIndex = this->bufferStartIndex;
    }
}

char KeyboardDriver::bufferPop()
{
    this->bufferPendingAmount--;
    this->bufferCurrentIndex--;
    char popChar = this->buffer[this->bufferCurrentIndex];

    if (this->bufferEndIndex < this->bufferCurrentIndex)
    {
        this->bufferCurrentIndex = this->bufferStartIndex;
    }

    // make sure pending amount cannot be less that 0
    if(bufferPendingAmount < 0){
        this->bufferPendingAmount = 0;
    }

    return popChar;
}

uint8_t KeyboardDriver::getBufferPendingAmount()
{
    return this->bufferPendingAmount;
}

uint32_t KeyboardDriver::HandleInterrupt(uint32_t esp)
{

    if (handler == 0)
    {
        return esp;
    }

    uint8_t key = dataport.Read();

    if (key < 0x80)
    {
        switch (key)
        {
        case 0x02:
            handler->OnKeyDown('1');
            this->bufferPush('1');
            break;
        case 0x03:
            handler->OnKeyDown('2');
            this->bufferPush('2');
            break;
        case 0x04:
            handler->OnKeyDown('3');
            this->bufferPush('3');
            break;
        case 0x05:
            handler->OnKeyDown('4');
            this->bufferPush('4');
            break;
        case 0x06:
            handler->OnKeyDown('5');
            this->bufferPush('5');
            break;
        case 0x07:
            handler->OnKeyDown('6');
            this->bufferPush('6');
            break;
        case 0x08:
            handler->OnKeyDown('7');
            this->bufferPush('7');
            break;
        case 0x09:
            handler->OnKeyDown('8');
            this->bufferPush('8');
            break;
        case 0x0A:
            handler->OnKeyDown('9');
            this->bufferPush('9');
            break;
        case 0x0B:
            handler->OnKeyDown('0');
            this->bufferPush('0');
            break;

        case 0x10:
            handler->OnKeyDown('q');
            this->bufferPush('q');
            break;
        case 0x11:
            handler->OnKeyDown('w');
            this->bufferPush('w');
            break;
        case 0x12:
            handler->OnKeyDown('e');
            this->bufferPush('e');
            break;
        case 0x13:
            handler->OnKeyDown('r');
            this->bufferPush('r');
            break;
        case 0x14:
            handler->OnKeyDown('t');
            this->bufferPush('t');
            break;
        case 0x15:
            handler->OnKeyDown('y');
            this->bufferPush('y');
            break;
        case 0x16:
            handler->OnKeyDown('u');
            this->bufferPush('u');
            break;
        case 0x17:
            handler->OnKeyDown('i');
            this->bufferPush('i');
            break;
        case 0x18:
            handler->OnKeyDown('o');
            this->bufferPush('o');
            break;
        case 0x19:
            handler->OnKeyDown('p');
            this->bufferPush('p');
            break;

        case 0x1E:
            handler->OnKeyDown('a');
            this->bufferPush('a');
            break;
        case 0x1F:
            handler->OnKeyDown('s');
            this->bufferPush('s');
            break;
        case 0x20:
            handler->OnKeyDown('d');
            this->bufferPush('d');
            break;
        case 0x21:
            handler->OnKeyDown('f');
            this->bufferPush('f');
            break;
        case 0x22:
            handler->OnKeyDown('g');
            this->bufferPush('g');
            break;
        case 0x23:
            handler->OnKeyDown('h');
            this->bufferPush('h');
            break;
        case 0x24:
            handler->OnKeyDown('j');
            this->bufferPush('j');
            break;
        case 0x25:
            handler->OnKeyDown('k');
            this->bufferPush('k');
            break;
        case 0x26:
            handler->OnKeyDown('l');
            this->bufferPush('l');
            break;
        case 0x2C:
            handler->OnKeyDown('y');
            this->bufferPush('y');
            break;
        case 0x2D:
            handler->OnKeyDown('x');
            this->bufferPush('x');
            break;
        case 0x2E:
            handler->OnKeyDown('c');
            this->bufferPush('c');
            break;
        case 0x2F:
            handler->OnKeyDown('v');
            this->bufferPush('v');
            break;
        case 0x30:
            handler->OnKeyDown('b');
            this->bufferPush('b');
            break;
        case 0x31:
            handler->OnKeyDown('n');
            this->bufferPush('n');
            break;
        case 0x32:
            handler->OnKeyDown('m');
            this->bufferPush('m');
            break;
        case 0x33:
            handler->OnKeyDown(',');
            this->bufferPush(',');
            break;
        case 0x34:
            handler->OnKeyDown('.');
            this->bufferPush('.');
            break;
        case 0x35:
            handler->OnKeyDown('-');
            this->bufferPush('-');
            break;
        case 0x1C:
            handler->OnKeyDown('\n');
            this->bufferPush('\n');
            break;
        case 0x39:
            handler->OnKeyDown(' ');
            this->bufferPush(' ');
            break;
        case 0x0E:
            handler->OnKeyDown('\b');
            this->bufferPush('\b');
            break;

        default:
            printf("KEYBOARD");
            printfHex(key);
            break;
        }
    }

    return esp;
}