#pragma once

#include <3ds.h>
#include <citro2d.h>
#include <memory>

#include <opus.h>
#include <opusfile.h>

#include "AudioManager.hpp"
#include "Entity.hpp"
#include "RenderWindow.hpp"
#include "Scene.hpp"
#include "SceneManager.hpp"

class SceneSplashScreen : public Scene {
public:
    SceneSplashScreen(SceneManager& a_sceneManager, AudioManager& a_audioManager, unsigned int a_spriteIndex);
    // SceneSplashScreen(SceneManager& a_sceneManager, unsigned int a_spriteIndex);

    // Implement the Scene interface
    void onCreate() override;
    void onDestroy() override;
    void onFocus() override;
    void onBlur() override; // not used

    void setNextSceneId(unsigned int a_id);

    void processInput() override;
    void update(float a_timeDelta) override;
    // void lateUpdate(float a_timeDelta) override;

    void drawUpper(RenderWindow& a_renderWindow) override;
    void drawLower(RenderWindow& a_renderWindow) override;

    // void draw(RenderWindow& a_renderWindow);
private:
    SceneManager& sceneManager;
    AudioManager& audioManager;

    unsigned int splashImageIndex;

    /// Which scene will we transition to after the splash screen?
    unsigned int nextSceneId;
    /// For how long should the splash screen stay active?
    float durationEnd;
    /// For how long has the splash screen been displayed?
    float durationElapsed;

    C2D_SpriteSheet spriteSheet;

    std::shared_ptr<Entity> splashImageEntity;
    std::shared_ptr<OggOpusFile> opusFile;
    unsigned int audioId;
};