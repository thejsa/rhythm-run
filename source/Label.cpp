/*
 * Rhythm Run for Nintendo 3DS
 * Lauren Kelly, 2020
 */

#include <3ds.h>
#include <citro2d.h>
#include <stdint.h>

#include "Label.hpp"

Label::Label(const char *p_text, float p_x, float p_y, u32 p_flags = 0, float p_scaleX = 0.5f, float p_scaleY = 0.5f, size_t p_bufSize = 0)
:textFlags(p_flags), posX(p_x), posY(p_y), scaleX(p_scaleX), scaleY(p_scaleY)
{
    // default buffer size to strlen, if not set
    if(!p_bufSize) p_bufSize = strlen(p_text);

    // Create a text buffer
    textBuf = C2D_TextBufNew(p_bufSize);

    // Parse the text string and store in the buffer
    C2D_TextParse(&text, textBuf, p_text);
    // Optimise the text string for fast rendering
    C2D_TextOptimize(&text);
};