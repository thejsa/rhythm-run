/*
 * Rhythm Run for Nintendo 3DS
 * Lauren Kelly, 2020
 */

#pragma once

#include "RenderWindow.hpp"
#include "Label.hpp"
#include "Entity.hpp"

#include <3ds.h>
#include <memory>

class Game
{
public:
    Game();
    
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
        this->drawUpper();
        this->drawLower();
    };

    bool isRunning() {  // Should the game continue running?
        hidScanInput();
        // Quit if APT says we should, or if user presses the START key
        return aptMainLoop() && !(hidKeysDown() & KEY_START);
    };
private:
    RenderWindow upperScreen;
    RenderWindow lowerScreen;

    std::shared_ptr<Label> myLabel;
    std::shared_ptr<Label> myLabel2;
    std::shared_ptr<Entity> mySprite;
    std::shared_ptr<Entity> mySprite2;
};