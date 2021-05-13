/*
 * Rhythm Run for Nintendo 3DS
 * Lauren Kelly, 2021
 */

#pragma once
#include "Vec2D.hpp"

class Rectangle {
public:
    /// Coordinate of the upper-left corner of the Rectangle.
    Vec2D topLeft;
    /// Coordinate of the lower-right corner of the Rectangle.
    Vec2D lowerRight;

    /// Rectangle constructor.
    Rectangle(Vec2D a_topLeft, Vec2D a_lowerRight)
        : topLeft(a_topLeft)
        , lowerRight(a_lowerRight) {};

    /// Returns the width of the rectangle.
    float getWidth() { return lowerRight.x - topLeft.x; }
    /// Returns the height of the rectangle.
    float getHeight() { return lowerRight.y - topLeft.y; }

    /// Returns true if this Rectangle intersects the given Rectangle.
    // inline bool doesIntersect(Rectangle a_rect) {
    //     // Two rectangles DO NOT overlap if and only if:
    //     // * One rectangle is above the top edge of the other; AND/OR
    //     // * One rectangle is further left than the left edge of the other.
    //     // Therefore, this checks that neither of the above conditions are met.

    //     return !(
    //         (topLeft.x >= a_rect.lowerRight.x) ||
    //         (a_rect.topLeft.x >= lowerRight.x) ||
    //         (topLeft.y <= a_rect.lowerRight.y) ||
    //         (a_rect.topLeft.y <= lowerRight.y)
    //     );
    // }

    /// Returns true if the two Rectangles provided as arguments do intersect.
    // static bool intersectionExists(Rectangle a_rect1, Rectangle a_rect2) {
    //     return (
    //         a_rect1.topLeft.x < a_rect2.lowerRight.x &&
    //         a_rect1.lowerRight.x > a_rect2.topLeft.x &&
    //         a_rect1.topLeft.y < a_rect2.lowerRight.y &&
    //         a_rect1.lowerRight.y > a_rect2.topLeft.y
    //     );
    // };

    /// Returns true if this Rectangle intersects with the Rectangle provided as the argument.
    bool doesIntersect(Rectangle a_rect)
    {
        return (
            this->topLeft.x<a_rect.lowerRight.x&& this->lowerRight.x> a_rect.topLeft.x && this->topLeft.y<a_rect.lowerRight.y&& this->lowerRight.y> a_rect.topLeft.y);
    }

    // bool doesIntersect(Rectangle a_rect) {
    //     return (
    //         (
    //             a_rect.topLeft.x >= topLeft.x &&
    //             a_rect.lowerRight.x <= lowerRight.x
    //         ) &&
    //         (
    //             a_rect.topLeft.y >= topLeft.y &&
    //             a_rect.lowerRight.y <= lowerRight.y
    //         )
    //     );
    // }

    /// Returns true if this Rectangle intersects the given Point.
    inline bool doesIntersect(Vec2D a_point)
    {
        // Construct a 1x1 rectangle of the point.
        return doesIntersect(Rectangle(a_point, a_point));
    }
};
