/*
 * Rhythm Run for Nintendo 3DS
 * Lauren Kelly, 2021
 */

#pragma once
#include "Rectangle.hpp"
#include "Vec2D.hpp"

class AABB {
public:
    Vec2D center;
    Vec2D halfSize;

    AABB() {};

    // Creates AABB from center vector and halfSize vector
    AABB(Vec2D a_center, Vec2D a_halfSize)
        : center(a_center)
        , halfSize(a_halfSize) {};

    AABB(Rectangle a_rect)
    {
        halfSize = {
            a_rect.getWidth() / 2,
            a_rect.getHeight() / 2
        };
        center = {
            a_rect.topLeft.x + halfSize.x,
            a_rect.topLeft.y + halfSize.y
        };
    };

    bool doesOverlap(AABB a_other)
    {
        if (abs(center.x - a_other.center.x) > halfSize.x + a_other.halfSize.x)
            return false;
        if (abs(center.y - a_other.center.y) > halfSize.y + a_other.halfSize.y)
            return false;

        return true;
    };
};