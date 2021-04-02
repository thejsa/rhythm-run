/*
 * Rhythm Run for Nintendo 3DS
 * Lauren Kelly, 2020
 */

#pragma once
#include <3ds.h>
#include <citro2d.h>
#include <stdint.h>

#include "Drawable.hpp"
#include "Geometry.hpp"

/**
 * Entity class
 */

class Entity : public Drawable
{
public:
    Entity(float a_x, float a_y, C2D_SpriteSheet a_spriteSheet, size_t a_spriteIndex,
    	float a_centerX, float a_centerY, float a_scaleX, float a_scaleY,
    	float a_rotation);
    Entity() {}; // default empty constructor

    // Getter functions
    inline float getX() { return sprite.params.pos.x; };
    inline float getY() { return sprite.params.pos.y; };
    inline float getCenterX() { return sprite.params.center.x; };
    inline float getCenterY() { return sprite.params.center.y; };
    inline float getScaleX() { return sprite.params.pos.w / sprite.image.subtex->width; };
    inline float getScaleY() { return sprite.params.pos.h / sprite.image.subtex->height; };
    inline float getRotation() { return sprite.params.angle; };
    inline float getRotationDegrees() { return sprite.params.angle*360.0f/M_TAU; };
    // Get scaled dimensions
    inline float getWidth() { return sprite.params.pos.w; };
    inline float getHeight() { return sprite.params.pos.h; };
    // Get original dimensions (before scaling)
    inline float get1xWidth() { return sprite.image.subtex->width; };
    inline float get1xHeight() { return sprite.image.subtex->height; };

    /// @brief Get bounding box
    Rectangle getRect();

    inline size_t getSpriteIndex() { return spriteIndex; };

    /// @brief Set sprite position on screen
    /// @param a_x,a_y Coordinates of the position in 2D space.
    inline void setPosition(float a_x, float a_y) { C2D_SpriteSetPos(&sprite, a_x, a_y); };
    /// @brief Set center position of sprite
    /// @param a_centerX,a_centerY Coordinates of the center point (0.0f thru 1.0f)
    inline void setCenter(float a_centerX, float a_centerY) { C2D_SpriteSetCenter(&sprite, a_centerX, a_centerY); };
    /// @brief Set absolute scale factor of sprite
    /// @param a_scaleX, a_scaleY Scale factor in each dimension
    inline void setScale(float a_scaleX, float a_scaleY) { C2D_SpriteSetScale(&sprite, a_scaleX, a_scaleY); };
    /// @brief Set sprite rotation (absolute)
    /// @param a_radians Rotation in radians
    inline void setRotation(float a_radians) { C2D_SpriteSetRotation(&sprite, a_radians); };
    /// @brief Set sprite rotation in degrees (absolute)
    /// @param a_degrees Rotation in degrees
    inline void setRotationDegrees(float a_degrees) { C2D_SpriteSetRotationDegrees(&sprite, a_degrees); };

    /// @brief Set the sprite graphic to be displayed
    /// @param a_index Index of the sprite to use
    /// @param a_spriteSheet Sprite sheet to use (defaults to the current)
    void setSprite(size_t a_index, C2D_SpriteSheet a_spriteSheet);

    /// @brief Move the sprite (relative)
    /// @param a_dX,a_dY Tranlation in each dimension
    inline void move(float a_dX, float a_dY) { C2D_SpriteMove(&sprite, a_dX, a_dY); };
    /// @brief Scale the sprite by some scale factor relative to its current size
    /// @param a_scaleX, a_scaleY Scale factor in each dimension
    inline void scale(float a_scaleX, float a_scaleY) { C2D_SpriteScale(&sprite, a_scaleX, a_scaleY); };
    // void scale(float a_scaleX, float a_scaleY) { C2D_SpriteSetScale(&sprite, getScaleX() * a_scaleX, getScaleY() * a_scaleY); };
    /// @brief Rotate the sprite (relative to current rotation)
    /// @param a_radians Rotation in radians
    inline void rotate(float a_radians) { C2D_SpriteRotate(&sprite, a_radians); };
    /// @brief Rotate the sprite in degrees (relative to current rotation)
    /// @param a_radians Rotation in degrees
    inline void rotateDegrees(float a_degrees) { C2D_SpriteRotate(&sprite, a_degrees); };
    
    /// @brief Draw the sprite
    inline bool draw() { return C2D_DrawSprite(&sprite); };
protected:
    /// @brief Current C2D sprite sheet
    C2D_SpriteSheet spriteSheet;
    /// @brief Index of sprite graphic currently being used from sheet
    size_t spriteIndex;
    /// @brief C2D sprite object
    C2D_Sprite sprite;
};