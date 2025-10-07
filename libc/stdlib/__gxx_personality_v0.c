#include <stdio.h>
#include <stdlib.h>

__attribute__((__noreturn__))
void __gxx_personality_v0(void){
    #if defined(__is_libk)
    // TODO: Add proper kernel panic.
        printf("kernel: panic: __gxx_personality_v0()\n");
        asm volatile("hlt");

    #else 
        // TODO: Abnornally terminate the procss as if by SIGABRT.
        printf("__gxx_personality_v0 ()");
    #endif

    while(1){
        __builtin_unreachable();
    }
}