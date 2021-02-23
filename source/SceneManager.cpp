#include "SceneManager.hpp"

/// Constructor
SceneManager::SceneManager()
:scenes(0), currentScene(nullptr), sceneCounter(0) {}

unsigned int SceneManager::addScene(std::shared_ptr<Scene> a_scene) {
	eprintf("id:%u p:%x\n", this->sceneCounter, a_scene);
    /// Add an id-scene pair to the scenes map
    auto insertedScene = this->scenes.insert(std::make_pair(this->sceneCounter, a_scene));

    /// Call onCreate of the scene we just inserted
    insertedScene.first->second->onCreate();

    /// Return, and then increment, the scene counter
    return this->sceneCounter++;
};

void SceneManager::removeScene(unsigned int a_sceneId) {
    // Find the id-scene pair for a_sceneId
	eprintf("id:%u\n", a_sceneId);
    auto scenePair = this->scenes.find(a_sceneId);

    if(scenePair != this->scenes.end()) {
        // If scene being removed is the current scene, set it to nullptr
        // to make sure we don't try to continue updating a nonexistant scene
        if(this->currentScene == scenePair->second) {
            this->currentScene = nullptr;
        }

        // Call scene's onDestroy method so it can deinit things, etc
        scenePair->second->onDestroy();

        // Erase the scene from the scenes map
        this->scenes.erase(scenePair);
    }
}

void SceneManager::switchFocusTo(unsigned int a_sceneId) {
    // Find the id-scene pair for a_sceneId
	eprintf("%u\n", a_sceneId);
    auto scenePair = this->scenes.find(a_sceneId);

    if(scenePair != this->scenes.end()) {
        // Call current scene's onBlur if necessary
        // eprintf("ptr:%u\n", scenePair->second);
        if(this->currentScene) {
            // eprintf("blurring old scene\n");
            this->currentScene->onBlur();
        }
    }
    
    // Set current scene to new scene
    this->currentScene = scenePair->second;

    // Call onFocus
    this->currentScene->onFocus();
}