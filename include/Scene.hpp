#pragma once

#include <3ds.h>
#include <citro2d.h>
#include <stdint.h>

#include <memory>
#include <vector>

#include "RenderWindow.hpp"
#include "Drawable.hpp"


class Scene
{
public:
    // Pure virtual function
    /// Called when the scene is initially created (one shot)
    virtual void onCreate() = 0;

    /// Called if/when the scene is destroyed (one shot)
    virtual void onDestroy() = 0;

    /// Called whenever the scene gains focus
    virtual void onFocus() {};

    /// Called whenever the scene loses focus ('blur')
    virtual void onBlur() {};

    // To be overridden as necessary
    virtual void processInput() {};
    virtual void update(float p_timeDelta) {};
    virtual void lateUpdate(float p_timeDelta);
    
    virtual void drawUpper(RenderWindow& p_renderWindow);
    virtual void drawLower(RenderWindow& p_renderWindow);

    virtual void draw(RenderWindow& p_renderWindowUpper, RenderWindow& p_renderWindowLower) {
        drawUpper(p_renderWindowUpper);
        drawLower(p_renderWindowLower);
    }

    // void begin() { renderWindow->beginDraw(); }

    // bool renderObjects() {
    //     renderWindow->beginDraw();

    //     for (auto&& obj : drawables) {
    //         obj->draw();
    //     }

    //     return true;
    // };
    // void init();
    // void clear(u32 p_color);

    // std::vector<std::shared_ptr<Drawable>> drawables;
// private:
    // RenderWindow* renderWindow;
    // gfxScreen_t screen;
    // gfx3dSide_t stereoSide;
};