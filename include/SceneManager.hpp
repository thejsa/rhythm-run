#pragma once

#include <3ds.h>
#include <stdint.h>

#include <memory>
#include <unordered_map>

#include "RenderWindow.hpp"
#include "Scene.hpp"

/// Finite state machine to handle scene management
class SceneManager
{
public:
    SceneManager();

    // Pass through to active scene
    void processInput() {
        if(currentScene) currentScene->processInput();
    };
    void update(float p_timeDelta) {
        if(currentScene) currentScene->update(p_timeDelta);
    };
    void lateUpdate(float p_timeDelta) {
        if(currentScene) currentScene->lateUpdate(p_timeDelta);
    };

    void drawUpper(RenderWindow& p_renderWindow) {
        if(currentScene) currentScene->drawUpper(p_renderWindow);
    };
    void drawLower(RenderWindow& p_renderWindow) {
        if(currentScene) currentScene->drawLower(p_renderWindow);
    };

    void draw(RenderWindow& p_renderWindowUpper, RenderWindow& p_renderWindowLower) {
        if(currentScene) currentScene->draw(p_renderWindowUpper, p_renderWindowLower);
    };

    // Add scene to the state machine, returning its ID
    unsigned int addScene(std::shared_ptr<Scene> p_scene);
    // Transition focus to scene
    void switchFocusTo(unsigned int p_sceneId);
    // Remove scene from the state machine
    void removeScene(unsigned int p_sceneId);

private:
    /// Stores all scenes
    std::unordered_map<unsigned int, std::shared_ptr<Scene>> scenes;
    /// Stores a reference to the current scene
    std::shared_ptr<Scene> currentScene;
    /// Stores the current scene ID, incremented whenever a scene is added
    unsigned int sceneCounter;
};