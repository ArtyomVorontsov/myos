#ifndef __MY__GUI__WINDOW_H
#define __MY__GUI__WINDOW_H

#include <gui/widget.hpp>
#include <driver/mouse.hpp>

class Window : public CompositeWidget
{
protected:
    bool Dragging;

public:
    Window(
        Widget *parent,
        int32_t x,
        int32_t y,
        int32_t w,
        int32_t h,
        int8_t r,
        int8_t g,
        int8_t b);

    void OnMouseDown(int32_t x, int32_t y, uint8_t button);
    void OnMouseUp(int32_t x, int32_t y, uint8_t button);
    void OnMouseMove(int32_t oldX, int32_t oldY, int32_t newX, int32_t newY);
};

#endif