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

class Button : public Entity
{
public:
    Button(float a_x, float a_y, C2D_SpriteSheet a_spriteSheet, size_t a_spriteIndex,
    	float a_centerX, float a_centerY, float a_scaleX, float a_scaleY,
    	float a_rotation);
    Button() {}; // default empty constructor

    /// @brief Add event listener
    /// @param a_eventType Event to listen for
    /// @param a_listener  Event listener
    void addEventListener(const touchEventKind_t a_eventType, const callback_t<touchEvent_t> &a_listener) {
        callbacks.push_back({a_listener, a_eventType});
    };
private:
    /// @brief Registered callbacks
    std::vector<touchEventListener_t> callbacks;
};