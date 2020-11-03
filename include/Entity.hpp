/*
 * Rhythm Run for Nintendo 3DS
 * Lauren Kelly, 2020
 */

#pragma once
#include <3ds.h>
#include <citro2d.h>
#include <stdint.h>

#include "Drawable.hpp"

/**
 * Entity class
 */

class Entity : public Drawable
{
public:
    Entity(float p_x, float p_y, C2D_SpriteSheet p_spriteSheet, size_t p_spriteIndex, float p_centerX, float p_centerY, float p_scaleX, float p_scaleY, float p_rotation);
    Entity() {}; // default empty constructor

    // Getter functions
    float getX() { return sprite.params.pos.x; };
    float getY() { return sprite.params.pos.y; };
    float getCenterX() { return sprite.params.center.x; };
    float getCenterY() { return sprite.params.center.y; };
    float getScaleX() { return sprite.params.pos.w / sprite.image.subtex->width; };
    float getScaleY() { return sprite.params.pos.h / sprite.image.subtex->height; };
    float getRotation() { return sprite.params.angle; };
    float getRotationDegrees() { return sprite.params.angle*360.0f/M_TAU; };
    // Get scaled dimensions
    float getWidth() { return sprite.params.pos.w; };
    float getHeight() { return sprite.params.pos.h; };
    // Get original dimensions (before scaling)
    float get1xWidth() { return sprite.image.subtex->width; };
    float get1xHeight() { return sprite.image.subtex->height; };
    size_t getSpriteIndex() { return spriteIndex; };

    /// @brief Set sprite position on screen
    /// @param p_x,p_y Coordinates of the position in 2D space.
    void setPosition(float p_x, float p_y) { C2D_SpriteSetPos(&sprite, p_x, p_y); };
    /// @brief Set center position of sprite
    /// @param p_centerX,p_centerY Coordinates of the center point (0.0f thru 1.0f)
    void setCenter(float p_centerX, float p_centerY) { C2D_SpriteSetCenter(&sprite, p_centerX, p_centerY); };
    /// @brief Set absolute scale factor of sprite
    /// @param p_scaleX, p_scaleY Scale factor in each dimension
    void setScale(float p_scaleX, float p_scaleY) { C2D_SpriteSetScale(&sprite, p_scaleX, p_scaleY); };
    /// @brief Set sprite rotation (absolute)
    /// @param p_radians Rotation in radians
    void setRotation(float p_radians) { C2D_SpriteSetRotation(&sprite, p_radians); };
    /// @brief Set sprite rotation in degrees (absolute)
    /// @param p_degrees Rotation in degrees
    void setRotationDegrees(float p_degrees) { C2D_SpriteSetRotationDegrees(&sprite, p_degrees); };

    /// @brief Set the sprite graphic to be displayed
    /// @param p_index Index of the sprite to use
    /// @param p_spriteSheet Sprite sheet to use (defaults to the current)
    void setSprite(size_t p_index, C2D_SpriteSheet p_spriteSheet);

    /// @brief Move the sprite (relative)
    /// @param p_dX,p_dY Tranlation in each dimension
    void move(float p_dX, float p_dY) { C2D_SpriteMove(&sprite, p_dX, p_dY); };
    /// @brief Scale the sprite by some scale factor relative to its current size
    /// @param p_scaleX, p_scaleY Scale factor in each dimension
    void scale(float p_scaleX, float p_scaleY) { C2D_SpriteScale(&sprite, p_scaleX, p_scaleY); };
    // void scale(float p_scaleX, float p_scaleY) { C2D_SpriteSetScale(&sprite, this->getScaleX() * p_scaleX, this->getScaleY() * p_scaleY); };
    /// @brief Rotate the sprite (relative to current rotation)
    /// @param p_radians Rotation in radians
    void rotate(float p_radians) { C2D_SpriteRotate(&sprite, p_radians); };
    /// @brief Rotate the sprite in degrees (relative to current rotation)
    /// @param p_radians Rotation in degrees
    void rotateDegrees(float p_degrees) { C2D_SpriteRotate(&sprite, p_degrees); };
    
    /// @brief Draw the sprite
    bool draw() { return C2D_DrawSprite(&sprite); };
private:
    /// @brief Current C2D sprite sheet
    C2D_SpriteSheet spriteSheet;
    /// @brief Index of sprite graphic currently being used from sheet
    size_t spriteIndex;
    /// @brief C2D sprite object
    C2D_Sprite sprite;
};