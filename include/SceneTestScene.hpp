#pragma once

#include <3ds.h>
#include <citro2d.h>

#include "Scene.hpp"


class SceneTestScene : Scene
{
public:
    // Implement the Scene interface
    void onCreate() override;
    void onDestroy() override;
    void onFocus() override;
    void onBlur() override;

    void processInput() override;
    void update(float p_timeDelta) override;
    void lateUpdate(float p_timeDelta) override;

    void drawUpper(RenderWindow& p_renderWindow) override;
    void drawLower(RenderWindow& p_renderWindow) override;

    // void draw(RenderWindow& p_renderWindow);
private:
    //
};