#ifndef __MY_OS__GUI__DESKTOP_H
#define __MY_OS__GUI__DESKTOP_H

#include <gui/widget.hpp>
#include <driver/mouse.hpp>

class Desktop : public CompositeWidget, public MouseEventHandler
{
protected:
    uint32_t MouseX;
    uint32_t MouseY;

public:
    Desktop(int32_t w, int32_t h, uint8_t r, uint8_t g, uint8_t b);

    void Draw(GraphicsContext *gc);
    void OnMouseDown(uint8_t button);
    void OnMouseUp(uint8_t button);
    void OnMouseMove(int x, int y);
};

#endif
