/*
 * Rhythm Run for Nintendo 3DS
 * Lauren Kelly, 2020
 */

#pragma once

#include "RenderWindow.hpp"
#include "Label.hpp"
#include "Entity.hpp"
#include "SceneManager.hpp"
#include "AudioManager.hpp"

#include <3ds.h>
#include <stdint.h>
#include <memory>

class Game
{
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

    /// Draw frame for upper screen
    void drawUpper();
    /// Draw frame for lower screen
    void drawLower();
    /// Render a full dual-screen frame
    void draw() {
        drawUpper();
        // drawLower();
    };
    
    bool isRunning() {  // Should the game continue running?
        // Quit if APT says we should, or if we've set the quit flag
        return aptMainLoop() && !quitFlag;
    };
private:
    RenderWindow upperScreen;
    RenderWindow lowerScreen;
    
    SceneManager sceneManager;
    AudioManager audioManager;
    
    u64 lastTime;
    float timeDelta;

    bool quitFlag;
};