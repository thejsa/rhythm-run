/*
 * Rhythm Run for Nintendo 3DS
 * Lauren Kelly, 2020
 */

#include "debug.hpp"

#include <3ds.h>
#include <citro2d.h>
#include <stdint.h>

#include "Label.hpp"

Label::Label(const char *a_text, float a_x, float a_y, u32 a_flags = 0, float a_scaleX = 0.5f, float a_scaleY = 0.5f, size_t a_bufSize = 0)
:textFlags(a_flags), posX(a_x), posY(a_y), scaleX(a_scaleX), scaleY(a_scaleY)
{
    // default buffer size to strlen, if not set
    if(!a_bufSize) a_bufSize = strlen(a_text) + 1;
    eprintf("init, str: %s, len: %d\n", a_text, a_bufSize);

    // Create a text buffer
    textBuf = C2D_TextBufNew(a_bufSize);

    // Parse the text string and store in the buffer
    C2D_TextParse(&text, textBuf, a_text);
    // Optimise the text string for fast rendering
    C2D_TextOptimize(&text);
    eprintf("done init\n");
};