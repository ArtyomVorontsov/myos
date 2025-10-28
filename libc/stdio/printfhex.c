
#include <stdio.h>

int printfHex(char key)
{
    char *foo = "0x00";
    char *hex = "0123456789ABCDEF";
    foo[2] = hex[(key >> 4) & 0xF];
    foo[3] = hex[key & 0xF];
    printf(foo);
}
