#ifndef __KEYBOARD_EVENT_HANDLER_H
#define __KEYBOARD_EVENT_HANDLER_H

class KeyboardEventHandler
{
public:
    KeyboardEventHandler();

    virtual void OnKeyDown(char);
    virtual void OnKeyUp(char);
    virtual void OnActivate();
};

#endif