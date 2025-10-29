#ifndef _STDIO_H
#define _STDIO_H 1

#include <sys/cdefs.h>
#include <types.h>

#define EOF (-1)

#ifdef __cplusplus
extern "C"
{
#endif
    int printf(const char *__restrict__ format, ...);
    int putchar(int);
    int puts(const char *);
    void printfHex(uint8_t);

#ifdef __cplusplus
}
#endif

#endif
