#include "types.h"
#include <event-handler/mouse-event-handler.hpp>

class PrintMouseEventHandler : public MouseEventHandler
{

public:
    int8_t x, y;
    virtual void OnActivate();
    virtual void OnMouseMove(int xOffset, int yOffset);
};