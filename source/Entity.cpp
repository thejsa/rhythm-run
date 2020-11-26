/*
 * Rhythm Run for Nintendo 3DS
 * Lauren Kelly, 2020
 */

#include <3ds.h>
#include <citro2d.h>
#include <stdint.h>

// Most getters/setters are defined in the header file to enhance performance optimisations
#include "Entity.hpp"

Entity::Entity(float p_x, float p_y, C2D_SpriteSheet p_spriteSheet,
	size_t p_spriteIndex = 0, float p_centerX = 0.5f, float p_centerY = 0.5f,
	float p_scaleX = 1.0f, float p_scaleY = 1.0f, float p_rotation = 0.0f)
:spriteSheet(p_spriteSheet), spriteIndex(p_spriteIndex)
{
    // Load specified sprite from specified spriteSheet
    C2D_SpriteFromSheet(&sprite, spriteSheet, spriteIndex);

    // Set position, scale
    C2D_SpriteSetCenter(&sprite, p_centerX, p_centerY);
    C2D_SpriteSetPos(&sprite, p_x, p_y);
    C2D_SpriteSetScale(&sprite, p_scaleX, p_scaleY);
    C2D_SpriteSetRotation(&sprite, p_rotation);
};

void Entity::setSprite(size_t p_index, C2D_SpriteSheet p_spriteSheet = NULL)
{
    spriteIndex = p_index;

    // If the caller passed a (valid) spritesheet param, use it
    if(p_spriteSheet != NULL) {
        spriteSheet = p_spriteSheet;
    }

    sprite.image = C2D_SpriteSheetGetImage(spriteSheet, spriteIndex);
}
