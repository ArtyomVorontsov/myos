#include "port.hpp"

Port::Port(uint16_t portnumber)
{
    this->portnumber = portnumber;
}

// 8 bit

Port8Bit::Port8Bit(uint16_t portnumber)
    : Port(portnumber)
{
}

void Port8Bit::Write(uint8_t data)
{
    Write8(portnumber, data);
}

uint8_t Port8Bit::Read()
{
    return Read8(portnumber);
}

// 8 bit slow

Port8BitSlow::Port8BitSlow(uint16_t portnumber)
    : Port8Bit(portnumber)
{
}

void Port8BitSlow::Write(uint8_t data)
{
    Write8Slow(portnumber, data);
}

// 16 bit

Port16Bit::Port16Bit(uint16_t portnumber)
    : Port(portnumber)
{
}

void Port16Bit::Write(uint16_t data)
{
    Write16(portnumber, data);
}

uint16_t Port16Bit::Read()
{
    return Read16(portnumber);
}

// 16 bit slow

Port16BitSlow::Port16BitSlow(uint16_t portnumber)
    : Port16Bit(portnumber)
{
}

void Port16BitSlow::Write(uint16_t data)
{
    Write16Slow(portnumber, data);
}

// 8 bit

Port32Bit::Port32Bit(uint16_t portnumber)
    : Port(portnumber)
{
}

void Port32Bit::Write(uint32_t data)
{
    Write32(portnumber, data);
}

uint32_t Port32Bit::Read()
{
    return Read32(portnumber);
}
