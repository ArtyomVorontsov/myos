#ifndef __KERNEL_TTY_H
#define __KERNEL_TTY_H

void terminal_initialize(void);
void terminal_putchar(char c);
void terminal_write(const char* data, size_t size);
void terminal_writestring(const char* data);

#endif
