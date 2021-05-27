/*
 * Rhythm Run for Nintendo 3DS
 * Lauren Kelly, 2021
 */

#pragma once

#include "Geometry.hpp"

class MovingObject {
public:
    MovingObject()
        : accel({ 0, 0 })
        , oldSpeed({ 0, 0 })
        , speed({ 0, 0 })
        , pushedRightWall(false)
        , pushesRightWall(false)
        , pushedLeftWall(false)
        , pushesLeftWall(false)
        , wasOnGround(false)
        , isOnGround(false)
        , wasAtCeiling(false)
        , isAtCeiling(false) {};

    // positions
    Vec2D oldPosition;
    Vec2D position;

    // acceleration
    Vec2D accel;

    // speeds
    Vec2D oldSpeed;
    Vec2D speed;

    // object scale
    // Vec2D scale;

    // Axis-Aligned BBox and offset
    AABB myAABB;
    Vec2D aabbOffset;

    // Walls logic
    bool pushedRightWall;
    bool pushesRightWall;

    bool pushedLeftWall;
    bool pushesLeftWall;

    bool wasOnGround;
    bool isOnGround;

    bool wasAtCeiling;
    bool isAtCeiling;

    virtual void update(float a_timeDelta) = 0;
    void updatePhysics(float a_timeDelta);
};