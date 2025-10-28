#include <stdio.h>
#include <event-handler/keyboard-event-handler.hpp>

class PrintKeyboardEventHandler : public KeyboardEventHandler
{
public:
    void OnKeyDown(char c);
};
