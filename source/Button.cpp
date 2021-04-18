/*
 * Rhythm Run for Nintendo 3DS
 * Lauren Kelly, 2020, 2021
 */

#include <3ds.h>
#include <citro2d.h>
#include <stdint.h>

// Most getters/setters are defined in the header file to enhance performance optimisations
#include "Button.hpp"

void Button::setSprite(size_t a_index, C2D_SpriteSheet a_spriteSheet)
{
    spriteIndex = a_index;
    if (!isPressed) {
        sprite.image = C2D_SpriteSheetGetImage(spriteSheet, spriteIndex);
    }
};

void Button::setSprite(size_t a_index, size_t a_index_pressed,
    C2D_SpriteSheet a_spriteSheet)
{
    spriteIndex = a_index;
    spriteIndex_pressed = a_index_pressed;

    if (!isPressed) {
        sprite.image = C2D_SpriteSheetGetImage(spriteSheet, spriteIndex);
    } else {
        sprite.image = C2D_SpriteSheetGetImage(spriteSheet, spriteIndex_pressed);
    }
};

void Button::setPressed(bool a_newState)
{
    // If there's no change in state, return
    if (isPressed == a_newState)
        return;

    isPressed = a_newState;

    if (a_newState == true) {
        sprite.image = C2D_SpriteSheetGetImage(spriteSheet, spriteIndex_pressed);
        move(2.0f, 2.0f);
    } else {
        sprite.image = C2D_SpriteSheetGetImage(spriteSheet, spriteIndex);
        // setSprite(spriteIndex, spriteSheet);
        move(-2.0f, -2.0f);
    }
}