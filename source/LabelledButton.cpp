/*
 * Rhythm Run for Nintendo 3DS
 * Lauren Kelly, 2021
 */

#include <3ds.h>
#include <citro2d.h>
#include <stdint.h>
#include <math.h>

// Most getters/setters are defined in the header file to enhance performance optimisations
#include "LabelledButton.hpp"

LabelledButton::LabelledButton(float a_x, float a_y, C2D_SpriteSheet a_spriteSheet,
        size_t a_spriteIndex, size_t a_spriteIndex_pressed,
    	float a_centerX, float a_centerY,
        const char *a_text,
        float a_scaleX, float a_scaleY,
    	float a_rotation,
        u32 a_textColor)
    : Button(a_x, a_y, a_spriteSheet, a_spriteIndex, a_spriteIndex_pressed,
             a_centerX, a_centerY, a_scaleX, a_scaleY, a_rotation) {
    // Point topLeft = getRect().topLeft;
    // Point lowerRight = getRect().lowerRight;
    Point newPosition = calculatePosition();

    float fontScale = 0.75f * getHeight() / 30.0f; // 3ds font height = 30px
    // float fontScale = 0.75f;
    eeprintf("Font scale: %.02f\n", fontScale);

    // float fontBaseline = 25.0f * fontScale;
    // // float fontBaseline = 24.0f;
    // eeprintf("Font base : %.02f\n", fontBaseline);

    eeprintf("Coordinates: %.02f, %.02f\n",
        newPosition.x,
        newPosition.y
    );

    /*
    textLabel = std::make_shared<Label>("Gameplay Test", 160, 48+24,
		C2D_AtBaseline | C2D_AlignCenter,
		0.75f, 0.75f, // font size
		C2D_Color32(0x00, 0xff, 0x00, 0xff),
		0
	);
    */

    textLabel = std::make_shared<Label>(a_text,
        newPosition.x, newPosition.y,
        C2D_AtBaseline | C2D_AlignCenter,
        fontScale * a_scaleY, fontScale * a_scaleY, // intentional both Y
        a_textColor, strlen(a_text) + 1
    );
}