#ifndef __MYOS_DRIVERS_VGA_H
#define __MYOS_DRIVERS_VGA_H

#include <types.h>
#include <hardwirecommunication/port.hpp>
#include <driver/driver.hpp>

class VideoGrapicsArray
{
protected:
    Port8Bit miscPort;
    Port8Bit crtcIndexPort;
    Port8Bit crtcDataPort;
    Port8Bit sequencerIndexPort;
    Port8Bit sequencerDataPort;
    Port8Bit grapicsControllerIndexPort;
    Port8Bit grapicsControllerDataPort;
    Port8Bit attributeControllerIndexPort;
    Port8Bit attributeControllerReadPort;
    Port8Bit attributeControllerWritePort;
    Port8Bit attributeControllerResetPort;

    void WriteRegisters(uint8_t *registers);
    uint8_t *GetFrameBufferSegment();

    virtual uint8_t GetColorIndex(uint8_t r, uint8_t g, uint8_t b);

    virtual void InitScreen();

public:
    VideoGrapicsArray();

    virtual void RenderScreen();
    virtual void PutPixelOnScreen(uint32_t x, uint32_t y, uint8_t r, uint8_t g, uint8_t b);
    virtual bool SupportsMode(uint32_t width, uint32_t height, uint32_t colorDepth);
    virtual bool SetMode(uint32_t width, uint32_t height, uint32_t colorDepth);
    virtual void PutPixel(uint32_t x, uint32_t y, uint8_t r, uint8_t g, uint8_t b);
    virtual void PutPixel(int32_t x, int32_t y, uint8_t colorIndex);

    virtual void FillRectangle(uint32_t x, uint32_t y, uint32_t w, uint32_t h, uint8_t r, uint8_t g, uint8_t b);
};

#endif