#pragma once

#include <3ds.h>
#include <citro2d.h>
#include <stdint.h>

#include <memory>
#include <vector>

#include "Drawable.hpp"
#include "RenderWindow.hpp"

class Scene {
public:
    Scene(RenderWindow* p_renderWindow)
        : renderWindow(p_renderWindow) {};

    void begin() { renderWindow->beginDraw(); }

    bool renderObjects()
    {
        renderWindow->beginDraw();

        for (auto&& obj : drawables) {
            obj->draw();
        }

        return true;
    };
    // void init();
    // void clear(u32 p_color);

    std::vector<std::shared_ptr<Drawable>> drawables;

private:
    RenderWindow* renderWindow;
    // gfxScreen_t screen;
    // gfx3dSide_t stereoSide;
};