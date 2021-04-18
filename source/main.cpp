/*
 * Rhythm Run for Nintendo 3DS
 * Lauren Kelly, 2020
 */

#include "debug.hpp"

#include "Game.hpp"
#include <3ds.h>

int main(int argc_, char* argv_[])
{
    // #ifdef DEBUG
    // printf("gdbHioDevInit %d\n", gdbHioDevInit());
    gdbHioDevInit();
    gdbHioDevRedirectStdStreams(true, true, true);
    eprintf("Hello from 3DS!\n");
    // #endif

    // Init libs
    gfxInitDefault();
    romfsInit();

    // Init console on touchscreen
    // consoleInit(GFX_BOTTOM, NULL);
    // consoleInit(GFX_TOP, NULL);
    // eprintf("inited console\n");

    // enable New3ds speedup
    osSetSpeedupEnable(true);

    eprintf("Init C2D\n");

    C3D_Init(C3D_DEFAULT_CMDBUF_SIZE);
    C3D_FrameRate(60.0f);
    C2D_Init(C2D_DEFAULT_MAX_OBJECTS);
    C2D_Prepare();

    // Game loop
    Game game;

    while (game.isRunning()) {
        // eprintf("aptMainLoop tick\n");
        game.processInput();
        game.update();
        game.lateUpdate();
        game.draw();
    }

    C2D_Fini();
    C3D_Fini();
    gfxExit();

    // #ifdef DEBUG
    gdbHioDevExit();
    // #endif

    return 0;
}