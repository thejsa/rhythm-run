#pragma once

#include "debug.hpp"

#include <3ds.h>
#include <citro2d.h>
#include <stdint.h>
#include <memory>

#include "Drawable.hpp"

class RenderWindow
{
public:
    RenderWindow(const gfxScreen_t a_screen = GFX_BOTTOM, const gfx3dSide_t a_stereoSide = GFX_LEFT)
    :renderTarget(NULL), screen(a_screen), stereoSide(a_stereoSide)
    {
        eprintf("Init %d/%d\n", a_screen, a_stereoSide);
        renderTarget = C2D_CreateScreenTarget(a_screen, a_stereoSide);

        // beginDraw();
        // clear(C2D_Color32(0xFF, 0x44, 0x77, 0xFF));
        // endDraw();
    };

    /// Update
    void update() { ((void)0); };

    /// Begin drawing
    void beginDraw() {
        // eprintf("Begin Draw\n");

		C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
        C2D_SceneBegin(renderTarget);

        // clear(C2D_Color32(0xFF, 0x44, 0x77, 0xFF));
    };

    /// Draw something
    void draw(const std::shared_ptr<Drawable> a_drawable) {
        a_drawable->draw();
    };

    /// End drawing
    void endDraw() {
        // eprintf("EndDraw\n");
        C3D_FrameEnd(0);

        // sync to 60fps
        float gpuTime = C3D_GetDrawingTime();
        float cpuTime = C3D_GetProcessingTime();
        // gfxFlushBuffers();
        if (gpuTime > 1000.0f / 30.f || cpuTime > 1000.0f / 30.0f) {
            printf("!! TOO SLOW !!\n");
        } else {
            gspWaitForVBlank();
        }
        printf("\x1b[29;1HGPU: %.02f ms; CPU: %.02f ms  ", gpuTime, cpuTime);
        // eprintf("")
        // gfxSwapBuffers();
    };

    /// Clear the window with a particular colour
    void clear(u32 a_color) {
        // eprintf("clear!\n");
        C2D_TargetClear(renderTarget, a_color);
    };
private:
    C3D_RenderTarget* renderTarget;
    gfxScreen_t screen;
    gfx3dSide_t stereoSide;
};