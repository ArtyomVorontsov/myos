#include <stdio.h>

#if defined(__islibk)
#include <kernel/tty.h>
#endif

int putchar(int ic)
{
#if defined(__is_libk)
    char c = (char)ic;
    terminal_write(&c, sizeof(c));
#else
    // TODO: Implement stdio and write system call.
#endif
    return ic;
}