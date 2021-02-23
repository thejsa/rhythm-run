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
    Label(const char *a_text, float a_x, float a_y, u32 a_flags, float a_scaleX, float a_scaleY, size_t a_bufSize);
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
        return getWidth() / 2.0f;
    };
    float getCenterY(){
        return getHeight() / 2.0f;
    };

    void setPosition(float a_x, float a_y) { posX = a_x; posY = a_y; };

    void move(float a_dX, float a_dY) {
        posX += a_dX;
        posY += a_dY;
    };
    void scale(float a_scaleX, float a_scaleY) {
        scaleX *= a_scaleX;
        scaleY *= a_scaleY;
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