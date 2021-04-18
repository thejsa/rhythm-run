/*
 * Rhythm Run for Nintendo 3DS
 * Lauren Kelly, 2020
 */

#pragma once

#include "AudioManager.hpp"
#include "Entity.hpp"
#include "Label.hpp"
#include "RenderWindow.hpp"
#include "SceneManager.hpp"

#include <3ds.h>
#include <memory>
#include <stdint.h>

class Game {
public:
    Game();

    /// Step the timer
    void stepTimer();

    /// Input processing code
    void processInput();
    /// Standard update code
    void update();
    /// Late update code, that must always run after update()
    void lateUpdate();
    /// Draw a frame
    void draw();

    /// should we quit the program?
    /// ask scene manager.
    inline bool shouldQuit()
    {
        return sceneManager.shouldQuit();
    }

    inline bool isRunning()
    { // Should the game continue running?
        // Quit if APT says we should, or if we've set the quit flag
        return aptMainLoop() && !shouldQuit();
    };

private:
    RenderWindow upperScreen;
    RenderWindow lowerScreen;

    SceneManager sceneManager;
    AudioManager audioManager;

    u64 lastTime;
    float timeDelta;
};