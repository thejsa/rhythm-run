/*
 * Rhythm Run for Nintendo 3DS
 * Lauren Kelly, 2020
 */

#pragma once

#include "debug.hpp"

#include <3ds.h>
#include <citro2d.h>
#include <stdint.h>

#include "Drawable.hpp"

/**
 * Label class
 */

class Label : public Drawable
{
public:
    Label(const char *p_text, float p_x, float p_y, u32 p_flags, float p_scaleX, float p_scaleY, size_t p_bufSize);
    Label() {}; // default empty constructor

    // Destructor -- clean up text buf
    ~Label() { C2D_TextBufDelete(textBuf); }

    // Label(const Label&) = delete;
    // Label(Label&&) = delete;
    // Label& operator=(const Label&) = delete;
    // Label& operator=(Label&&) = delete;

    // Getters
    float getX() { return posX; }
    float getY() { return posY; }
    float getWidth() {
        float width;
        C2D_TextGetDimensions(&text, scaleX, scaleY, &width, nullptr);
        return width;
    }
    float getHeight() {
        float height;
        C2D_TextGetDimensions(&text, scaleX, scaleY, nullptr, &height);
        return height;
    }
    float getCenterX() {
        return this->getWidth() / 2.0f;
    };
    float getCenterY(){
        return this->getHeight() / 2.0f;
    };

    void setPosition(float p_x, float p_y) { posX = p_x; posY = p_y; };

    void move(float p_dX, float p_dY) {
        posX += p_dX;
        posY += p_dY;
    };
    void scale(float p_scaleX, float p_scaleY) {
        scaleX *= p_scaleX;
        scaleY *= p_scaleY;
    };

    bool draw() {
        C2D_DrawText(&text, textFlags, posX, posY, 0.0f, scaleX, scaleY);
        return true;
    };
private:
    C2D_Text text;
    C2D_TextBuf textBuf;
    u32 textFlags;

    float posX, posY;
    float scaleX, scaleY;
};