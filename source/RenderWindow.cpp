#include <3ds.h>
#include <citro2d.h>
#include <iostream>

#include "RenderWindow.hpp"

RenderWindow::RenderWindow(const gfxScreen_t p_screen = GFX_BOTTOM, const gfx3dSide_t p_stereoSide = GFX_LEFT)
:renderTarget(NULL), screen(p_screen), stereoSide(p_stereoSide)
{
    renderTarget = C2D_CreateScreenTarget(p_screen, p_stereoSide);
};

/// @brief Helper to begin a 2D scene on this RenderWindow.
void RenderWindow::beginScene()
{
    C2D_SceneBegin(renderTarget);
}

void RenderWindow::clear(u32 p_color)
{
    C2D_TargetClear(renderTarget, p_color);
}