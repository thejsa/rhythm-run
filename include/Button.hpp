/*
 * Rhythm Run for Nintendo 3DS
 * Lauren Kelly, 2021
 */

#pragma once
#include <3ds.h>
#include <citro2d.h>
#include <stdint.h>

#include "Entity.hpp"
#include "Events.hpp"

/**
 * Button class
 */

class Button : public Entity {
public:
    Button(float a_x, float a_y, C2D_SpriteSheet a_spriteSheet,
        size_t a_spriteIndex, size_t a_spriteIndex_pressed,
        float a_centerX, float a_centerY,
        float a_scaleX, float a_scaleY,
        float a_rotation)
        : Entity(a_x, a_y, a_spriteSheet, a_spriteIndex, a_centerX, a_centerY,
            a_scaleX, a_scaleY, a_rotation)
        , isPressed(false)
        , spriteIndex_pressed(a_spriteIndex_pressed) {};
    Button() {}; // default empty constructor

    /// @brief Add event listener
    /// @param a_eventType Event to listen for
    /// @param a_listener  Event listener
    void addEventListener(const touchEventKind_t a_eventType, const callback_t<touchEvent_t>& a_listener)
    {
        callbacks.push_back({ a_listener, a_eventType });
    };

    /// @brief Set the sprite graphic to be displayed
    /// @param a_index Index of the sprite to use (normal)
    /// @param a_spriteSheet Sprite sheet to use (defaults to the current)
    void setSprite(size_t a_index, C2D_SpriteSheet a_spriteSheet);

    /// @brief Set the sprite graphic to be displayed
    /// @param a_index Index of the sprite to use (normal)
    /// @param a_index_pressed Index of the sprite to use (pressed state)
    /// @param a_spriteSheet Sprite sheet to use (defaults to the current)
    void setSprite(size_t a_index, size_t a_index_pressed, C2D_SpriteSheet a_spriteSheet);

    inline bool getPressed() { return isPressed; }
    void setPressed(bool a_newState);

protected:
    /// @brief Whether or not the button is in its pressed/held state.
    bool isPressed;
    /// @brief Index of sprite graphic currently being used from sheet
    size_t spriteIndex_pressed;
    /// @brief Registered callbacks
    std::vector<touchEventListener_t> callbacks;
};