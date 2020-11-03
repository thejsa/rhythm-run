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
    RenderWindow(const gfxScreen_t p_screen = GFX_BOTTOM, const gfx3dSide_t p_stereoSide = GFX_LEFT)
    :renderTarget(NULL), screen(p_screen), stereoSide(p_stereoSide)
    {
        eprintf("Init %d/%d\n", p_screen, p_stereoSide);
        renderTarget = C2D_CreateScreenTarget(p_screen, p_stereoSide);

        // this->beginDraw();
        // this->clear(C2D_Color32(0xFF, 0x44, 0x77, 0xFF));
        // this->endDraw();
    };

    /// Update
    void update() { ((void)0); };

    /// Begin drawing
    void beginDraw() {
        // eprintf("Begin Draw\n");

		C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
        C2D_SceneBegin(renderTarget);

        this->clear(C2D_Color32(0xFF, 0x44, 0x77, 0xFF));
    };

    /// Draw something
    void draw(const std::shared_ptr<Drawable> p_drawable) {
        p_drawable->draw();
    };

    /// End drawing
    void endDraw() {
        // eprintf("EndDraw\n");
        C3D_FrameEnd(0);
    };

    /// Clear the window with a particular colour
    void clear(u32 p_color) {
        // eprintf("clear!\n");
        C2D_TargetClear(renderTarget, p_color);
    };
private:
    C3D_RenderTarget* renderTarget;
    gfxScreen_t screen;
    gfx3dSide_t stereoSide;
};