/*
 * Rhythm Run for Nintendo 3DS
 * Lauren Kelly, 2020
 */

#include <3ds.h>

#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <vector>
#include <memory>

#include <time.h>

#include <citro2d.h>

#include "debug.h"
#include "Scene.hpp"
#include "Entity.hpp"
#include "Label.hpp"

#define MAX_SPRITES   384
#define SCREEN_WIDTH  400
#define SCREEN_HEIGHT 240
#define TAU 6.28f

static inline void justSpin(void) {
	while(aptMainLoop()) {
		// Respond to user input
		hidScanInput();
		if(hidKeysDown() & KEY_START) break; // return to hbmenu
	}
}

static C2D_SpriteSheet spriteSheet;
// super simple sprite
typedef struct
{
	std::shared_ptr<Entity> spr;
	float dx, dy; // velocity
	float sx, sy; // scale factor
} Sprite;
static Sprite sprites[MAX_SPRITES];

// static size_t numSprites = MAX_SPRITES/2;
static size_t numSprites = 3;

static float d_rotation = 0.05f;
static float gb_shouldSpriteScale = true;
static float gb_shouldSpritesDraw = true;
static float gb_shouldSpritesAnimate = true;

static void initSprites() {
	size_t numImages = C2D_SpriteSheetCount(spriteSheet);
	srand(time(NULL));

	for (size_t i = 0; i < MAX_SPRITES; i++)
	{
		Sprite* sprite = &sprites[i];

		// Random image, position, scale and speed
		auto e = std::make_shared<Entity>(rand() % SCREEN_WIDTH, rand() % SCREEN_HEIGHT, // random pos
			spriteSheet, rand() % numImages, // random image
			0.5f, 0.5f, // center
			rand()*1.0f/RAND_MAX, rand() * 1.0f/RAND_MAX,
			C3D_Angle(rand()/(float)RAND_MAX)
		);

		sprite->spr = e;
		
		sprite->dx = rand()*4.0f/RAND_MAX - 2.0f;
		sprite->dy = rand()*4.0f/RAND_MAX - 2.0f;

		sprite->sx = rand()*TAU/RAND_MAX;
		sprite->sy = rand()*TAU/RAND_MAX;
	}
};

static void moveSprites() {
	for (size_t i = 0; i < numSprites; i++)
	{
		Sprite* sprite = &sprites[i];
		sprite->spr->move(sprite->dx, sprite->dy);
		
		// exciting scaling
		// float old_x, old_y;
		// old_x = sprite->spr->getCenterX();
		// old_y = sprite->spr->getCenterY();

		sprite->spr->setScale(sin(sprite->sx)*1.0f, cos(sprite->sy)*1.0f);
		// sprite->spr->setPosition(old_x, old_y);

		sprite->spr->rotateDegrees(d_rotation);

		printf("\x1b[15;1HSx: %.1f / Sy: %.1f / degrees: %.2f", sprite->sx, sprite->sy, d_rotation);

		// float scaleFactor = sprite->spr->getScaleX();
		// if(scaleFactor >= 1.0f) {
		// 	scaleFactor = 0.0f;
		// }
		if(gb_shouldSpriteScale) {
			sprite->sx = fmod(sprite->sx + 0.1f, TAU);
			sprite->sy = fmod(sprite->sy + 0.1f, TAU);
		}

		// Check for collision with the screen boundaries
		if ((sprite->spr->getX() < sprite->spr->getWidth() / 2.0f && sprite->dx < 0.0f) ||
			(sprite->spr->getX() > (SCREEN_WIDTH-(sprite->spr->getWidth() / 2.0f)) && sprite->dx > 0.0f))
			sprite->dx = -sprite->dx;

		if ((sprite->spr->getY() < sprite->spr->getHeight() / 2.0f && sprite->dy < 0.0f) ||
			(sprite->spr->getY() > (SCREEN_HEIGHT-(sprite->spr->getHeight() / 2.0f)) && sprite->dy > 0.0f))
			sprite->dy = -sprite->dy;
	}
}

int main(int argc_, char *argv_[]) {
	// Init libs
	gfxInitDefault();
	romfsInit();

	// Init console on touchscreen
	consoleInit(GFX_BOTTOM, NULL);
	eprintf("inited console\n");
	
	C3D_Init(C3D_DEFAULT_CMDBUF_SIZE);
	C2D_Init(C2D_DEFAULT_MAX_OBJECTS);
	C2D_Prepare();

	// Initialise RenderWindows
	// printf("init renderwindow\n");
	RenderWindow upperScreen(GFX_TOP, GFX_LEFT);
	// RenderWindow lowerScreen(GFX_BOTTOM, GFX_LEFT);

	// Create scene
	Scene mainScene(&upperScreen);

	// printf("load sheet\n");
	// load sheet
	spriteSheet = C2D_SpriteSheetLoad("romfs:/gfx/sprites.t3x");
	if (!spriteSheet) {
		printf("failed to load sprite sheet\n");
		justSpin();
		// svcBreak(USERBREAK_PANIC);
		return 1;
	}

	// printf("init sprites\n");
	initSprites();

	const char *str = "Hello world!";
	std::shared_ptr<Label> l = std::make_shared<Label>(str, 24.0f, 24.0f, 0, 0.5f, 0.5f, 0);
	mainScene.drawables.push_back(l);

	for (size_t i = 0; i < numSprites; i++) {
		mainScene.drawables.push_back(sprites[i].spr);
	}

	printf("\x1b[8;1HPress Up to increment sprites");
	printf("\x1b[9;1HPress Down to decrement sprites");
	printf("\x1b[10;1HPress Left to decrease rotation");
	printf("\x1b[11;1HPress Right to increase rotation");
	printf("\x1b[12;1HPress A to toggle scaling");

	while(aptMainLoop()) {
		// Respond to user input
		hidScanInput();
		if(hidKeysDown() & KEY_START) break; // return to hbmenu

		u32 kHeld = hidKeysHeld();
		if ((kHeld & KEY_UP) && numSprites < MAX_SPRITES) {
			numSprites++;
			mainScene.drawables.push_back(sprites[numSprites - 1].spr);
		} else if ((kHeld & KEY_DOWN) && numSprites > 1) {
			numSprites--;
			mainScene.drawables.pop_back();
		}
		if ((kHeld & KEY_LEFT) && d_rotation > 0.0f) {
			d_rotation = d_rotation - 0.05f;
		} else if ((kHeld & KEY_RIGHT) && d_rotation < 180.0f) {
			d_rotation = d_rotation + 0.05f;
		}

		if(hidKeysDown() & KEY_A) {
			gb_shouldSpriteScale = !gb_shouldSpriteScale;
		}

		if(hidKeysDown() & KEY_X) {
			gb_shouldSpritesAnimate = !gb_shouldSpritesAnimate;
		}
		if(hidKeysDown() & KEY_Y) {
			gb_shouldSpritesDraw = !gb_shouldSpritesDraw;
		}


		// Render scene
		eprintf("begin frame\n");
		C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
		eprintf("begin scene\n");
		// upperScreen.beginScene();

		mainScene.begin();

		u32 clearColor = C2D_Color32(0xFF, 0x44, 0x77, 0xFF);
		// printf("clear screen\n");
		upperScreen.clear(clearColor);

		// printf("move sprites\n");

		if(gb_shouldSpritesAnimate) {
			eprintf("moving sprites\n");
			moveSprites();
		} else {
			eprintf("not moving sprites! nop\n");
		}
		printf("\x1b[1;1HSprites: %zu/%u\x1b[K", numSprites, MAX_SPRITES);
		printf("\x1b[2;1HCPU:     %6.2f%%\x1b[K", C3D_GetProcessingTime()*6.0f);
		printf("\x1b[3;1HGPU:     %6.2f%%\x1b[K", C3D_GetDrawingTime()*6.0f);
		printf("\x1b[4;1HCmdBuf:  %6.2f%%\x1b[K", C3D_GetCmdBufUsage()*100.0f);
		// for (size_t i = 0; i < numSprites; i++) {
		// 	// printf("draw sprite %d/%d\n", i, numSprites);
		// 	// C2D_DrawSprite(&sprites[i].spr);
		// 	sprites[i].spr->draw();
		// }
		// l.draw();

		if(gb_shouldSpritesDraw) {
			eprintf("rendering scene objects\n");
			mainScene.renderObjects();
		} else {
			eprintf("not drawing sprites! nop\n");
		}

		C3D_FrameEnd(0);
	}

	// De-init libs
	C2D_Fini();
	C3D_Fini();
	gfxExit();

	return 0;
}