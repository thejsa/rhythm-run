/*
 * Rhythm Run for Nintendo 3DS
 * Lauren Kelly, 2021
 */

#pragma once
#include <3ds.h>
#include <citro2d.h>
#include <stdint.h>
#include <memory>

#include "Button.hpp"
#include "Label.hpp"

/**
 * LabelledButton class
 */

class LabelledButton : public Button
{
public:
    LabelledButton(float a_x, float a_y, C2D_SpriteSheet a_spriteSheet,
        size_t a_spriteIndex, size_t a_spriteIndex_pressed,
    	float a_centerX, float a_centerY,
        const char *a_text,
        float a_scaleX, float a_scaleY,
    	float a_rotation,
        u32 a_textColor);
    LabelledButton() {}; // default empty constructor

    /// Set the state (pressed/not pressed)
    // inline void setPressed(bool a_newState) {
    //     if(isPressed == a_newState) return;

    //     Button::setPressed(a_newState);
    //     if(a_newState == true) {
    //         textLabel->move(2.0f, 2.0f);
    //     } else {
    //         textLabel->move(-2.0f, -2.0f);
    //     }
    // };

    /// Draw the LabelledButton
    inline bool draw() {
        Button::draw();

        Point newPosition = calculatePosition();

        // eeprintf("textLabel->setPosition %.02f, %.02f\n",
        //     floor(newPosition.x),
        //     floor(newPosition.y)
        // );
        textLabel->setPosition(
            floor(newPosition.x),
            floor(newPosition.y)
        );

        textLabel->draw();
        return true;
    };

    /// @brief Move the button (relative)
    /// @param a_dX,a_dY Tranlation in each dimension
    inline void move(float a_dX, float a_dY) {
        Button::move(a_dX, a_dY);
        textLabel->move(a_dX, a_dY);
    };
    /// @brief Scale the button by some scale factor relative to its current size
    /// @param a_scaleX, a_scaleY Scale factor in each dimension
    inline void scale(float a_scaleX, float a_scaleY) {
        Button::scale(a_scaleX, a_scaleY);
        textLabel->scale(a_scaleX, a_scaleY);
    }

    /// Set label text
    // inline void setText(const char *a_text, size_t a_bufSize = 0) { textLabel->setText(a_text, a_bufSize); };
    /// Set label color
    // inline void setColor(u32 a_color = 0) { textLabel->setColor(a_color); };
    /// @brief The button label
    std::shared_ptr<Label> textLabel;
protected:
    Point calculatePosition() {
        Point topLeft = getRect().topLeft;
        
        float fontScale = 0.75f * getHeight() / 30.0f; // 3ds font height = 30px
        // float fontScale = 0.75f;
        // eeprintf("Font scale: %.02f\n", fontScale);

        float fontBaseline = 25.0f * fontScale;
        // float fontBaseline = 24.0f;
        // eeprintf("Font base : %.02f\n", fontBaseline);

        return Point{
            topLeft.x + (getWidth() / 2),
            topLeft.y + fontBaseline + (getHeight() / 8)
        };
    }
};