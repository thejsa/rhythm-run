/*
 * Rhythm Run for Nintendo 3DS
 * Lauren Kelly, 2021
 */

#pragma once

class Constants {
public:
    static constexpr float GRAVITY = 8.0f * -9.81f;
    static constexpr float TERMINAL_VELOCITY = 1200.0f * -GRAVITY;
    static constexpr float JUMP_SPEED = 0.25f * -GRAVITY;
    static constexpr float MIN_JUMP_SPEED = 10.0f;
    static constexpr float WALK_SPEED = 2.5f;
};