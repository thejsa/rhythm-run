#pragma once

#include <3ds.h>
#include <citro2d.h>
#include <stdint.h>

class RenderWindow
{
public:
    RenderWindow(const gfxScreen_t p_screen = GFX_BOTTOM, const gfx3dSide_t p_stereoSide = GFX_LEFT)
    :renderTarget(NULL), screen(p_screen), stereoSide(p_stereoSide)
    {
        renderTarget = C2D_CreateScreenTarget(p_screen, p_stereoSide);
    };

    /// @brief Helper to begin a 2D scene on this RenderWindow.
    void beginScene() { C2D_SceneBegin(renderTarget); };
    void clear(u32 p_color) { C2D_TargetClear(renderTarget, p_color); }
private:
    C3D_RenderTarget* renderTarget;
    gfxScreen_t screen;
    gfx3dSide_t stereoSide;
};