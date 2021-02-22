/*
 * Rhythm Run for Nintendo 3DS
 * Lauren Kelly, 2020
 */

#pragma once
#include <stdint.h>

typedef struct Point {
    float x;
    float y;
} Point;

class Rectangle {
    /// Coordinate of the upper-left corner of the Rectangle.
    Point topLeft;
    /// Coordinate of the lower-right corner of the Rectangle.
    Point lowerRight;

    /// Rectangle constructor.
    Rectangle(Point *a_topLeft, Point *a_lowerRight)
        : topLeft(*a_topLeft), lowerRight(*a_lowerRight) { };

    /// Returns true if this Rectangle intersects the given Rectangle.
    inline bool doesIntersect(Rectangle *a_rect) {
        // Two rectangles DO NOT overlap if and only if:
        // * One rectangle is above the top edge of the other; AND/OR
        // * One rectangle is further left than the left edge of the other.
        // Therefore, this checks that neither of the above conditions are met.

        return !(
            (this->topLeft.x >= a_rect->lowerRight.x) ||
            (a_rect->topLeft.x >= this->lowerRight.x) ||
            (this->topLeft.y <= a_rect->lowerRight.y) ||
            (a_rect->topLeft.y <= this->lowerRight.y)
        );
    }

    /// Returns true if this Rectangle intersects the given Point.
    inline bool doesIntersect(Point *a_point) {
        // Construct a 1x1 rectangle of the point.
        return doesIntersect(&Rectangle(a_point, a_point));
    }
};