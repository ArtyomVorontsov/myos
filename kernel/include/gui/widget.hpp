#ifndef __MY_OS__GUI__WIDGET_H
#define __MY_OS__GUI__WIDGET_H

#include <types.h>
#include <common/graphics-context.hpp>
#include <driver/keyboard.hpp>

class Widget : public KeyboardEventHandler
{
protected:
    Widget *parent;
    int32_t x;
    int32_t y;
    int32_t w;
    int32_t h;

    uint8_t r;
    uint8_t g;
    uint8_t b;

    bool Focusable;

public:
    Widget(
        Widget *parent,
        int32_t x,
        int32_t y,
        int32_t w,
        int32_t h,
        uint8_t r,
        uint8_t g,
        uint8_t b);

    virtual void GetFocus(Widget *widget);
    virtual void ModelToScreen(int32_t &x, int32_t &y);
    virtual void Draw(GraphicsContext *gc);
    virtual void OnMouseDown(int32_t x, int32_t y, uint8_t button);
    virtual void OnMouseUp(int32_t x, int32_t y, uint8_t button);
    virtual void OnMouseMove(int32_t oldX, int32_t oldY, int32_t newX, int32_t newY);
    virtual bool ContainsCoordinate(int32_t x, int32_t y);
};

class CompositeWidget : public Widget
{

private:
    Widget *children[100];
    int numChildren;
    Widget *focussedChild;

public:
    CompositeWidget(
        Widget *parent,
        int32_t x,
        int32_t y,
        int32_t w,
        int32_t h,
        uint8_t r,
        uint8_t g,
        uint8_t b);

    virtual void GetFocus(Widget *widget);
    virtual bool AddChild(Widget *child);
    virtual void Draw(GraphicsContext *gc);
    virtual void OnMouseDown(int32_t x, int32_t y, uint8_t button);
    virtual void OnMouseUp(int32_t x, int32_t y, uint8_t button);
    virtual void OnMouseMove(int32_t oldX, int32_t oldY, int32_t newX, int32_t newY);
    virtual void OnKeyDown(char c);
    virtual void OnKeyUp(char c);
};

#endif