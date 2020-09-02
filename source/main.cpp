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

#include <time.h>

#include <citro2d.h>

#include "widgets/Object.hpp"
#include "RenderWindow.hpp"
#include "Entity.hpp"

#define MAX_SPRITES   768
#define SCREEN_WIDTH  400
#define SCREEN_HEIGHT 240
#define TAU 6.28f

C2D_TextBuf g_textBuf;
C2D_Text g_staticText;

static C2D_SpriteSheet spriteSheet;
// super simple sprite
typedef struct
{
	Entity spr;
	float dx, dy; // velocity
	float sx, sy; // scale factor
} Sprite;
static Sprite sprites[MAX_SPRITES];
// static size_t numSprites = MAX_SPRITES/2;
static size_t numSprites = 1;

static float d_rotation = 1.8f;

static void initSprites() {
	size_t numImages = C2D_SpriteSheetCount(spriteSheet);
	srand(time(NULL));

	for (size_t i = 0; i < MAX_SPRITES; i++)
	{
		Sprite* sprite = &sprites[i];

		// Random image, position, scale and speed
		Entity e(rand() % SCREEN_WIDTH, rand() % SCREEN_HEIGHT, // random pos
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
}
static void moveSprites() {
	for (size_t i = 0; i < numSprites; i++)
	{
		Sprite* sprite = &sprites[i];
		sprite->spr.move(sprite->dx, sprite->dy);
		
		// exciting scaling
		// float old_x, old_y;
		// old_x = sprite->spr.getCenterX();
		// old_y = sprite->spr.getCenterY();

		sprite->spr.setScale(sin(sprite->sx)*1.0f, cos(sprite->sy)*1.0f);
		// sprite->spr.setPosition(old_x, old_y);

		sprite->spr.rotateDegrees(d_rotation);

		printf("\x1b[11;1HSx: %.1f / Sy: %.1f / degrees: %.1f", sprite->sx, sprite->sy, d_rotation);

		// float scaleFactor = sprite->spr.getScaleX();
		// if(scaleFactor >= 1.0f) {
		// 	scaleFactor = 0.0f;
		// }
		sprite->sx = fmod(sprite->sx + 0.1f, TAU);
		sprite->sy = fmod(sprite->sy + 0.1f, TAU);

		// Check for collision with the screen boundaries
		if ((sprite->spr.getX() < sprite->spr.getWidth() / 2.0f && sprite->dx < 0.0f) ||
			(sprite->spr.getX() > (SCREEN_WIDTH-(sprite->spr.getWidth() / 2.0f)) && sprite->dx > 0.0f))
			sprite->dx = -sprite->dx;

		if ((sprite->spr.getY() < sprite->spr.getHeight() / 2.0f && sprite->dy < 0.0f) ||
			(sprite->spr.getY() > (SCREEN_HEIGHT-(sprite->spr.getHeight() / 2.0f)) && sprite->dy > 0.0f))
			sprite->dy = -sprite->dy;
	}
}

// static void sceneInit(void) {
// 	// Create text buffer
// 	g_textBuf = C2D_TextBufNew(4096);

// 	// Parse text string
// 	C2D_TextParse(&g_staticText, g_textBuf, "Hello, world!");

// 	// Optimise text string
// 	C2D_TextOptimize(&g_staticText);
// }

// static void sceneRender(void) {
// 	// Draw text string
// 	float textPosX = 400.0f - 16.0f - g_staticText.width * 0.75f; // right justify
// 	C2D_DrawText(&g_staticText, 0, textPosX, 8.0f, 0.5f, 0.5f, 0.5f);
// }

// static void sceneExit(void) {
// 	// Delete text buffer
// 	C2D_TextBufDelete(g_textBuf);
// }

static inline void justSpin(void) {
	while(aptMainLoop()) {
		// Respond to user input
		hidScanInput();
		if(hidKeysDown() & KEY_START) break; // return to hbmenu
	}
}

int main(int argc_, char *argv_[]) {
	// Init libs
	gfxInitDefault();
	romfsInit();

	// Init console on touchscreen
	consoleInit(GFX_BOTTOM, NULL);
	// printf("inited console\n");
	
	C3D_Init(C3D_DEFAULT_CMDBUF_SIZE);
	C2D_Init(C2D_DEFAULT_MAX_OBJECTS);
	C2D_Prepare();
	// printf("inited c3d/c2d and prepared\n");

	// Create screen
	// C3D_RenderTarget *top = C2D_CreateScreenTarget(GFX_TOP, GFX_LEFT);

	// Initialise scene
	// sceneInit();

	// widgets::Object myObj(GFX_TOP, 128.0f, 128.0f, 64.0f, 64.0f, 24.0f, 24.0f);
	// printf("Object width: %f\n", myObj.getWidth());

	// // Main loop
	// while(aptMainLoop()) {
	// 	hidScanInput();

	// 	// Respond to user input
	// 	if (hidKeysDown() & KEY_START) break; // in order to return to hbmenu

	// 	// Render scene
	// 	C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
	// 	// paint background colour
	// 	C2D_TargetClear(top, C2D_Color32(0xFF, 0x44, 0x77, 0xFF));
	// 	C2D_SceneBegin(top);
	// 	sceneRender();
	// 	C3D_FrameEnd(0);
	// }

	// cleanup
	// De-init the scene
	// sceneExit();

	// Initialise RenderWindows
	// printf("init renderwindow\n");
	RenderWindow upperScreen(GFX_TOP, GFX_LEFT);
	// RenderWindow lowerScreen(GFX_BOTTOM, GFX_LEFT);

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

	printf("\x1b[8;1HPress Up to increment sprites");
	printf("\x1b[9;1HPress Down to decrement sprites");

	while(aptMainLoop()) {
		// Respond to user input
		hidScanInput();
		if(hidKeysDown() & KEY_START) break; // return to hbmenu

		u32 kHeld = hidKeysHeld();
		if ((kHeld & KEY_UP) && numSprites < MAX_SPRITES) {
			numSprites++;
		} else if ((kHeld & KEY_DOWN) && numSprites > 1) {
			numSprites--;
		}
		if ((kHeld & KEY_LEFT) && d_rotation > 0.0f) {
			d_rotation = d_rotation - 0.1f;
		} else if ((kHeld & KEY_RIGHT) && d_rotation < 180.0f) {
			d_rotation = d_rotation + 0.1f;
		}


		// Render scene
		// printf("begin frame\n");
		C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
		// printf("begin scene\n");
		upperScreen.beginScene();
		u32 clearColor = C2D_Color32(0xFF, 0x44, 0x77, 0xFF);
		// printf("clear screen\n");
		upperScreen.clear(clearColor);

		// printf("move sprites\n");
		moveSprites();
		printf("\x1b[1;1HSprites: %zu/%u\x1b[K", numSprites, MAX_SPRITES);
		printf("\x1b[2;1HCPU:     %6.2f%%\x1b[K", C3D_GetProcessingTime()*6.0f);
		printf("\x1b[3;1HGPU:     %6.2f%%\x1b[K", C3D_GetDrawingTime()*6.0f);
		printf("\x1b[4;1HCmdBuf:  %6.2f%%\x1b[K", C3D_GetCmdBufUsage()*100.0f);
		for (size_t i = 0; i < numSprites; i++) {
			// printf("draw sprite %d/%d\n", i, numSprites);
			// C2D_DrawSprite(&sprites[i].spr);
			sprites[i].spr.draw();
		}
		C3D_FrameEnd(0);
	}

	// De-init libs
	C2D_Fini();
	C3D_Fini();
	gfxExit();

	return 0;
}