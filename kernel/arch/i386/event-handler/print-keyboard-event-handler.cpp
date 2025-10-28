#include <stdio.h>
#include <event-handler/print-keyboard-event-handler.hpp>

void PrintKeyboardEventHandler::OnKeyDown(char c)
{
    char *foo = " ";
    foo[0] = c;
    printf(foo);
};
