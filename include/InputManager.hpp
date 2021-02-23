#pragma once

#include <3ds.h>
#include <stdint.h>

#include <memory>
#include <unordered_map>

#include "Events.hpp"

/// Input manager
class InputManager
{
public:
    InputManager();

    // Pass through to active scene
    void processInput() {
        if(currentScene) currentScene->processInput();
    };
    void update(float a_timeDelta) {
        if(currentScene) currentScene->update(a_timeDelta);
    };
    void lateUpdate(float a_timeDelta) {
        if(currentScene) currentScene->lateUpdate(a_timeDelta);
    };

    void drawUpper(RenderWindow& a_renderWindow) {
        if(currentScene) currentScene->drawUpper(a_renderWindow);
    };
    void drawLower(RenderWindow& a_renderWindow) {
        if(currentScene) currentScene->drawLower(a_renderWindow);
    };

    void draw(RenderWindow& a_renderWindowUpper, RenderWindow& a_renderWindowLower) {
        if(currentScene) currentScene->draw(a_renderWindowUpper, a_renderWindowLower);
    };

    // Add scene to the state machine, returning its ID
    unsigned int addScene(std::shared_ptr<Scene> a_scene);
    // Transition focus to scene
    void switchFocusTo(unsigned int a_sceneId);
    // Remove scene from the state machine
    void removeScene(unsigned int a_sceneId);

private:
    /// Stores all scenes
    std::unordered_map<unsigned int, std::shared_ptr<Scene>> scenes;
    /// Stores a reference to the current scene
    std::shared_ptr<Scene> currentScene;
    /// Stores the current scene ID, incremented whenever a scene is added
    unsigned int sceneCounter;
};