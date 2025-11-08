#ifndef __MY_OS__GUI__TEXT_H
#define __MY_OS__GUI__TEXT_H

#include <types.h>
#include <common/graphics-context.hpp>

class Text
{
public:
    static void printSymbol(uint8_t n, uint32_t X, uint32_t Y, GraphicsContext *gc);
    static void printAlphabet(GraphicsContext *gc);
    static void printString(char *c, GraphicsContext *gc);
};

#endif