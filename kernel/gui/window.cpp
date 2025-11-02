#include <gui/window.hpp>

Window::Window(
    Widget *parent,
    int32_t x,
    int32_t y,
    int32_t w,
    int32_t h,
    int8_t r,
    int8_t g,
    int8_t b) : CompositeWidget(parent, x, y, w, h, r, g, b)
{
    Dragging = false;
}

void Window::OnMouseDown(int32_t x, int32_t y, uint8_t button)
{
    Dragging = button == 1;

    CompositeWidget::OnMouseDown(x, y, button);
}
void Window::OnMouseUp(int32_t x, int32_t y, uint8_t button)
{
    Dragging = false;

    CompositeWidget::OnMouseUp(x, y, button);
}

void Window::OnMouseMove(int32_t oldX, int32_t oldY, int32_t newX, int32_t newY)
{
    if (Dragging)
    {
        this->x += newX - oldX;
        this->y += newY - oldY;
    }

    CompositeWidget::OnMouseMove(oldX, oldY, newX, newY);
}
