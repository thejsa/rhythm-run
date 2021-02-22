#pragma once

#include <3ds.h>
#include <citro2d.h>
#include <memory>

#include <opusfile.h>
#include <opus.h>

#include "SceneManager.hpp"
#include "AudioManager.hpp"
#include "RenderWindow.hpp"
#include "Scene.hpp"
#include "Entity.hpp"

class SceneGameplay : public Scene
{
public:
	SceneGameplay(SceneManager& p_sceneManager,
	AudioManager& p_audioManager, const char* p_trackName);
	// SceneGameplay(SceneManager& p_sceneManager, unsigned int p_spriteIndex);
	
    // Implement the Scene interface
    void onCreate() override;
    void onDestroy() override;
    void onFocus() override;
    void onBlur() override; // not used
	
	void setNextSceneId(unsigned int p_id);

    void processInput() override;
    void update(float p_timeDelta) override;
    // void lateUpdate(float p_timeDelta) override;

    void drawUpper(RenderWindow& p_renderWindow) override;
    void drawLower(RenderWindow& p_renderWindow) override;

    // void draw(RenderWindow& p_renderWindow);
private:
    SceneManager& sceneManager;
    AudioManager& audioManager;

	const char* trackName;

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

    bool isJumping;
    float velocityX, velocityY;
    float accelX, accelY;
};