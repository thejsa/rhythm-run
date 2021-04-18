/*
 * Rhythm Run for Nintendo 3DS
 * Lauren Kelly, 2020
 */

#pragma once

/**
 * Drawable abstract class
 * Provides an interface to be implemented by objects which are drawn.
 */

class Drawable {
public:
    // Pure virtual function, since this class shouldn't be instantiated
    // Instead, subclasses must implement this themselves
    /// @brief Draw the sprite
    virtual bool draw() = 0;
};