/*
 * Rhythm Run for Nintendo 3DS
 * Lauren Kelly, 2021
 */

#pragma once

#include <3ds.h>
#include <citro2d.h>
#include <deque>
#include <fstream>
#include <iostream>
#include <memory>
#include <utility>

#include <opus.h>
#include <opusfile.h>

#include "AudioManager.hpp"
#include "Beat.hpp"
#include "BeatMap.hpp"
#include "Entity.hpp"
#include "Label.hpp"
#include "Player.hpp"
#include "RenderWindow.hpp"
#include "Scene.hpp"
#include "SceneGameplayEnd.hpp"
#include "SceneManager.hpp"

class SceneGameplay : public Scene {
public:
    SceneGameplay(SceneManager& a_sceneManager,
        AudioManager& a_audioManager, const char* a_trackName);
    // SceneGameplay(SceneManager& a_sceneManager, unsigned int a_spriteIndex);

    // Implement the Scene interface
    void onCreate() override;
    void onDestroy() override;
    void onFocus() override;
    void onBlur() override; // not used

    void setWinSceneId(unsigned int a_id);
    void setWinScene(std::shared_ptr<SceneGameplayEnd> winScene);
    void setPauseSceneId(unsigned int a_id);

    void processInput() override;
    void update(float a_timeDelta) override;
    // void lateUpdate(float a_timeDe   lta) override;

    void draw(RenderWindow& a_renderWindowUpper, RenderWindow& a_renderWindowLower) override;

    // void draw(RenderWindow& a_renderWindow);
private:
    SceneManager& sceneManager;
    AudioManager& audioManager;
    std::shared_ptr<SceneGameplayEnd> winScene;

    const char* trackName;

    /// Which scene will we transition to after the game ends?
    unsigned int winSceneId;
    /// Which scene will we transition to when the game pauses?
    unsigned int pauseSceneId;
    /// For how long should the splash screen stay active?
    float durationEnd;
    /// For how long has the splash screen been displayed?
    float durationElapsed;

    C2D_SpriteSheet spriteSheet;
    C2D_SpriteSheet bgSpriteSheet;

    std::shared_ptr<Entity> levelBgEntity;

    std::shared_ptr<Player> myPlayer;

    // beats on screen
    std::deque<std::shared_ptr<Beat>> beatQueue;

    std::shared_ptr<Entity> platform1Entity;
    std::shared_ptr<Entity> platform2Entity;
    std::shared_ptr<Entity> platform3Entity;
    std::shared_ptr<Entity> platform4Entity;

    std::shared_ptr<Label> debugLabel;
    std::shared_ptr<Label> scoreLabel;

    std::shared_ptr<OggOpusFile> opusFile;
    unsigned int audioId;

    BeatMap myBeatmap;
    long playerScore;
};