#include <stdio.h>
#include <stdlib.h>

__attribute__((__noreturn__))
void malloc(void){
    #if defined(__is_libk)
    // TODO: Add proper kernel panic.
        printf("kernel: panic: malloc()\n");
        asm volatile("hlt");

    #else 
        // TODO: Abnornally terminate the procss as if by SIGABRT.
        printf("malloc ()");
    #endif

    while(1){
        __builtin_unreachable();
    }
}