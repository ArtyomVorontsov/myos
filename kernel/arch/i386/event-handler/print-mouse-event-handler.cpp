#include "types.h"
#include <event-handler/print-mouse-event-handler.hpp>

void PrintMouseEventHandler::OnActivate()
{

    uint16_t *VideoMemory = (uint16_t *)0xb8000;

    x = 40;
    y = 12;

    // Invert color on startup position
    VideoMemory[80 * y + x] = (VideoMemory[80 * y + x] & 0x0F00) << 4 |
                              (VideoMemory[80 * y + x] & 0x0F000) >> 4 |
                              (VideoMemory[80 * y + x] & 0x00FF);
};

void PrintMouseEventHandler::OnMouseMove(int xOffset, int yOffset)
{
    // Invert color of previous position
    static uint16_t *VideoMemory = (uint16_t *)0xb8000;
    VideoMemory[80 * y + x] = (VideoMemory[80 * y + x] & 0x0F00) << 4 |
                              (VideoMemory[80 * y + x] & 0x0F000) >> 4 |
                              (VideoMemory[80 * y + x] & 0x00FF);

    x += xOffset;
    if (x >= 80)
        x = 79;
    if (x < 0)
        x = 0;
    y -= yOffset;
    if (y >= 25)
        y = 24;
    if (y < 0)
        y = 0;

    // Invert color of current position
    VideoMemory[80 * y + x] = (VideoMemory[80 * y + x] & 0x0F00) << 4 |
                              (VideoMemory[80 * y + x] & 0x0F000) >> 4 |
                              (VideoMemory[80 * y + x] & 0x00FF);
};
