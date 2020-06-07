/*
 * Rhythm Run for Nintendo 3DS
 * Lauren Kelly, 2020
 */

#include <3ds.h>

#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cmath>

#include <citro2d.h>

#include "widgets/Object.hpp"

C2D_TextBuf g_textBuf;
C2D_Text g_staticText;

static void sceneInit(void) {
	// Create text buffer
	g_textBuf = C2D_TextBufNew(4096);

	// Parse text string
	C2D_TextParse(&g_staticText, g_textBuf, "Hello, world!");

	// Optimise text string
	C2D_TextOptimize(&g_staticText);
}

static void sceneRender(void) {
	// Draw text string
	float textPosX = 400.0f - 16.0f - g_staticText.width * 0.75f; // right justify
	C2D_DrawText(&g_staticText, 0, textPosX, 8.0f, 0.5f, 0.5f, 0.5f);
}

static void sceneExit(void) {
	// Delete text buffer
	C2D_TextBufDelete(g_textBuf);
}

int main(int argc_, char *argv_[]) {
	// Init libs
	gfxInitDefault();
	
	C3D_Init(C3D_DEFAULT_CMDBUF_SIZE);
	C2D_Init(C2D_DEFAULT_MAX_OBJECTS);
	C2D_Prepare();

	// Create screen
	C3D_RenderTarget *top = C2D_CreateScreenTarget(GFX_TOP, GFX_LEFT);
	// Init console on touchscreen
	consoleInit(GFX_BOTTOM, NULL);

	// Initialise scene
	sceneInit();

	widgets::Object myObj(GFX_TOP, 128.0f, 128.0f, 64.0f, 64.0f, 24.0f, 24.0f);
	printf("Object width: %f\n", myObj.getWidth());

	// Main loop
	while(aptMainLoop()) {
		hidScanInput();

		// Respond to user input
		if (hidKeysDown() & KEY_START) break; // in order to return to hbmenu

		// Render scene
		C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
		// paint background colour
		C2D_TargetClear(top, C2D_Color32(0xFF, 0x44, 0x77, 0xFF));
		C2D_SceneBegin(top);
		sceneRender();
		C3D_FrameEnd(0);
	}

	// cleanup
	// De-init the scene
	sceneExit();

	// De-init libs
	C2D_Fini();
	C3D_Fini();
	gfxExit();

	return 0;
}