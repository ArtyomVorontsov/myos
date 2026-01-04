#include <stdbool.h>
#include <string.h>
#include <types.h>
#include <kernel/tty.h>

#include "vga.h"

static const size_t VGA_WIDTH = 80;
static const size_t VGA_HEIGHT = 25;
static uint16_t *const VGA_MEMORY = (uint16_t *)0xB8000;

static size_t terminal_row;
static size_t terminal_column;
static uint8_t terminal_color;
static uint16_t *terminal_buffer;

static inline void outb(uint16_t port, uint8_t val)
{
    __asm__ volatile("outb %0, %1" : : "a"(val), "Nd"(port));
}

void terminal_initialize(void)
{
    terminal_row = 0;
    terminal_column = 0;
    terminal_color = vga_entry_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
    terminal_buffer = VGA_MEMORY;
    for (size_t y = 0; y < VGA_HEIGHT; y++)
    {
        for (size_t x = 0; x < VGA_WIDTH; x++)
        {
            const size_t index = y * VGA_WIDTH + x;
            terminal_buffer[index] = vga_entry(' ', terminal_color);
        }
    }
}

void terminal_setcolor(uint8_t color)
{
    terminal_color = color;
}

void terminal_putentryat(unsigned char c, uint8_t color, size_t x, size_t y)
{
    const size_t index = y * VGA_WIDTH + x;
    terminal_buffer[index] = vga_entry(c, color);
}

void terminal_update_hw_cursor(void)
{
    uint16_t pos = terminal_row * VGA_WIDTH + terminal_column;

    outb(0x3D4, 0x0F);
    outb(0x3D5, (uint8_t)(pos & 0xFF));

    outb(0x3D4, 0x0E);
    outb(0x3D5, (uint8_t)((pos >> 8) & 0xFF));
}

// TODO: fix
// void terminal_scroll(int line)
// {
//     int loop;
//     char c;

//     for (loop = line * (VGA_WIDTH * 2) + 0xB8000; loop < VGA_WIDTH * 2; loop++)
//     {
//         c = *loop;
//         *(loop - (VGA_WIDTH * 2)) = c;
//     }
// }

void terminal_delete_last_line()
{
    int x, *ptr;

    for (x = 0; x < VGA_WIDTH * 2; x++)
    {
        ptr = (int *)(0xB8000 + (VGA_WIDTH * 2) * (VGA_HEIGHT - 1) + x);
        *ptr = 0;
    }
}

void terminal_putchar(char c)
{
    int line;
    unsigned char uc = c;

    if (c == '\b')
    {
        terminal_column--;
        terminal_putentryat(' ', terminal_color, terminal_column, terminal_row);
        terminal_column--;
    }
    else if (c == '\n')
    {
        terminal_column = VGA_WIDTH;
    }
    else
    {
        terminal_putentryat(uc, terminal_color, terminal_column, terminal_row);
    }

    if (++terminal_column >= VGA_WIDTH)
    {
        terminal_column = 0;
        if (++terminal_row >= VGA_HEIGHT)
        {
            terminal_clear_screen();
            terminal_row = 0;
            terminal_column = 0;
        }
    }

    terminal_update_hw_cursor();
}

void terminal_write(const char *data, size_t size)
{
    for (size_t i = 0; i < size; i++)
    {
        terminal_putchar(data[i]);
    }
}

void terminal_writestring(const char *data)
{
    terminal_write(data, strlen(data));
}

void terminal_clear_screen()
{
    for (int i = 0; i < VGA_HEIGHT; i++)
    {
        for (int j = 0; j < VGA_WIDTH; j++)
        {
            terminal_putentryat(' ', terminal_color, j, i);
        }
    }
}