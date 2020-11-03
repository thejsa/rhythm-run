/*
 * Rhythm Run for Nintendo 3DS
 * Lauren Kelly, 2020
 */

#include "debug.hpp"

#include <3ds.h>
#include "Game.hpp"

int main(int argc_, char *argv_[]) {
	// Init libs
	gfxInitDefault();
	romfsInit();

	// Init console on touchscreen
	// consoleInit(GFX_BOTTOM, NULL);
	eprintf("inited console\n");

	eprintf("Init C2D\n");

	C3D_Init(C3D_DEFAULT_CMDBUF_SIZE);
	C2D_Init(C2D_DEFAULT_MAX_OBJECTS);
	C2D_Prepare();

	// Game loop
	Game game;

	while(game.isRunning()) {
		// Respond to user input
		hidScanInput();
		if(hidKeysDown() & KEY_START) break; // return to hbmenu

		// eprintf("aptMainLoop tick\n");

		game.update();
		game.lateUpdate();
		game.draw();
	}

	C2D_Fini();
	C3D_Fini();
	gfxExit();

	return 0;
}