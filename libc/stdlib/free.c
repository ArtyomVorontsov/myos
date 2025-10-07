#include <stdio.h>
#include <stdlib.h>

__attribute__((__noreturn__))
void free(void){
    #if defined(__is_libk)
    // TODO: Add proper kernel panic.
        printf("kernel: panic: free()\n");
        asm volatile("hlt");

    #else 
        // TODO: Abnornally terminate the procss as if by SIGABRT.
        printf("free ()");
    #endif

    while(1){
        __builtin_unreachable();
    }
}