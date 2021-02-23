/*
 * Rhythm Run for Nintendo 3DS
 * Lauren Kelly, 2020, 2021
 */

#include <3ds.h>
#include <citro2d.h>
#include <stdint.h>

// Most getters/setters are defined in the header file to enhance performance optimisations
#include "Entity.hpp"

Entity::Entity(float a_x, float a_y, C2D_SpriteSheet a_spriteSheet,
	size_t a_spriteIndex = 0, float a_centerX = 0.5f, float a_centerY = 0.5f,
	float a_scaleX = 1.0f, float a_scaleY = 1.0f, float a_rotation = 0.0f)
:spriteSheet(a_spriteSheet), spriteIndex(a_spriteIndex)
{
    // Load specified sprite from specified spriteSheet
    C2D_SpriteFromSheet(&sprite, spriteSheet, spriteIndex);

    // Set position, scale
    C2D_SpriteSetCenter(&sprite, a_centerX, a_centerY);
    C2D_SpriteSetPos(&sprite, a_x, a_y);
    C2D_SpriteSetScale(&sprite, a_scaleX, a_scaleY);
    C2D_SpriteSetRotation(&sprite, a_rotation);
};

void Entity::setSprite(size_t a_index, C2D_SpriteSheet a_spriteSheet = NULL)
{
    spriteIndex = a_index;

    // If the caller passed a (valid) spritesheet param, use it
    if(a_spriteSheet != NULL) {
        spriteSheet = a_spriteSheet;
    }

    sprite.image = C2D_SpriteSheetGetImage(spriteSheet, spriteIndex);
}

Rectangle Entity::getRect() {
    float halfWidth = getWidth() / 2;
    float halfHeight = getHeight() / 2;

    return Rectangle(
        // top left corner
        {
            sprite.params.pos.x - halfWidth,
            sprite.params.pos.y - halfHeight
        },
        // lower right corner
        {
            sprite.params.pos.x + halfWidth,
            sprite.params.pos.y + halfHeight
        }
    );
}