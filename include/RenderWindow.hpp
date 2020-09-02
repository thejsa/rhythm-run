#pragma once

#include <3ds.h>
#include <citro2d.h>
#include <stdint.h>

class RenderWindow
{
public:
    RenderWindow(const gfxScreen_t p_screen, const gfx3dSide_t p_3dSide);
    void beginScene();
    void clear(u32 p_color);
private:
    C3D_RenderTarget* renderTarget;
    gfxScreen_t screen;
    gfx3dSide_t stereoSide;
};