#include <gui/desktop.hpp>

Desktop::Desktop(int32_t w, int32_t h, uint8_t r, uint8_t g, uint8_t b)
    : CompositeWidget(0, 0, 0, w, h, r, g, b),
      MouseEventHandler()
{
    MouseX = w / 2;
    MouseY = h / 2;
}

void Desktop::Draw(GraphicsContext *gc)
{

    CompositeWidget::Draw(gc);

    for (int i = 0; i < 4; i++)
    {
        gc->PutPixelOnScreen(MouseX - i, MouseY, 0xFF, 0xFF, 0xFF);
        gc->PutPixelOnScreen(MouseX + i, MouseY, 0xFF, 0xFF, 0xFF);
        gc->PutPixelOnScreen(MouseX, MouseY - i, 0xFF, 0xFF, 0xFF);
        gc->PutPixelOnScreen(MouseX, MouseY + i, 0xFF, 0xFF, 0xFF);
    }
}

void Desktop::OnMouseDown(uint8_t button)
{
    CompositeWidget::OnMouseDown(MouseX, MouseY, button);
}

void Desktop::OnMouseUp(uint8_t button)
{
    CompositeWidget::OnMouseUp(MouseX, MouseY, button);
}

void Desktop::OnMouseMove(int x, int y)
{
    x /= 2;
    y /= 2;

    int32_t newMouseX = MouseX + x;
    if (newMouseX < 0)
        newMouseX = 0;
    if (newMouseX >= w)
        newMouseX = w - 1;

    int32_t newMouseY = MouseY + y;
    if (newMouseY < 0)
        newMouseY = 0;
    if (newMouseY >= h)
        newMouseY = h - 1;

    CompositeWidget::OnMouseMove(MouseX, MouseY, newMouseX, newMouseY);

    MouseX = newMouseX;
    MouseY = newMouseY;
}
