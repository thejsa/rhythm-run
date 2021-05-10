/*
 * Rhythm Run for Nintendo 3DS
 * Lauren Kelly, 2021
 */

#pragma once
#include <functional>

#include "Geometry.hpp"

template <typename T>
using callback_t = std::function<void(T)>;

/// Touch event types
typedef enum touchEventKind_t {
    TOUCH_START = 0,
    TOUCH_END = 1,
    TOUCH_MOVE = 2,
} touchEventKind_t;

/// Touch event
typedef struct touchEvent_t {
    touchEventKind_t kind;
    Vec2D pos;
} touchEvent_t;

/// Touch event listener
typedef struct touchEventListener_t {
    callback_t<touchEvent_t> cb;
    touchEventKind_t kind;
} touchEventListener_t;