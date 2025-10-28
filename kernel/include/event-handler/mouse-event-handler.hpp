#ifndef __MOUSE_EVENT_HANDLER_H
#define __MOUSE_EVENT_HANDLER_H
#include <types.h>

class MouseEventHandler
{
public:
    MouseEventHandler();

    virtual void OnActivate();
    virtual void OnMouseDown(uint8_t button);
    virtual void OnMouseUp(uint8_t button);
    virtual void OnMouseMove(int x, int y);
};

#endif