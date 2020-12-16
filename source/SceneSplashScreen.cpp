#include "SceneSplashScreen.hpp"

SceneSplashScreen::SceneSplashScreen(SceneManager& p_sceneManager,
	AudioManager& p_audioManager, unsigned int p_spriteIndex)
:sceneManager(p_sceneManager), audioManager(p_audioManager), splashImageIndex(p_spriteIndex),
	nextSceneId(0), durationEnd(5.0f), durationElapsed(0.0f)
{};

void SceneSplashScreen::onCreate()
{
	// Load sprite sheet
	eprintf("OnCreate\n");

	this->spriteSheet = C2D_SpriteSheetLoad("romfs:/gfx/splash.t3x");
	if (!spriteSheet) {
		eprintf("failed to load sprite sheet\n");
		// justSpin();
		svcBreak(USERBREAK_PANIC);
	}
	
	// Create sprite
	// float scale;
	// if(this->splashImageIndex == 2) {
	// 	scale = 4.0f;
	// } else {
	// 	scale = 1.0f;
	// }
	this->splashImageEntity = std::make_shared<Entity>(200, 120, // centre of screen
		this->spriteSheet, this->splashImageIndex, // which sheet & image to load
		0.5f, 0.5f, // sprite's origin
		1.0f, 1.0f, // scale
		0.0f //rotation
	);

	// Play audio
	// soloud.init();
	// sample.load("romfs:/sample.wav");
	int error = 0;
	this->opusFile = std::shared_ptr<OggOpusFile>(op_open_file ("romfs:/sample.opus", &error), op_free);

	if (error)
	{
		eprintf("Failed to open file! error: %d\n", error);
		this->opusFile = nullptr;
	} else {
		this->audioId = this->audioManager.addFile(this->opusFile);
	}

	eprintf("Fini\n");
}

void SceneSplashScreen::onFocus()
{
	// eprintf("Focus\n");
	// Reset duration timer
	this->durationElapsed = 0.0f;

	if(this->opusFile) {
		this->audioManager.switchFileTo(this->audioId);
		this->audioManager.play();
	}
	
	// sample.load("romfs:/sample.wav");
	// soloud.play(sample);
}

void SceneSplashScreen::onBlur()
{
	if(this->opusFile) {
		this->audioManager.pause();
	}
}

void SceneSplashScreen::onDestroy() {
	((void)0);
	// soloud.deinit();
	// if(this->opusFile) op_free(this->opusFile);
	// smart pointer means we don't need to?

	eprintf("OnDestroy\n");
}

void SceneSplashScreen::setNextSceneId(unsigned int p_id)
{
	eprintf("%u\n", p_id);
	this->nextSceneId = p_id;
}

void SceneSplashScreen::update(float p_timeDelta)
{
	// eprintf("Update, delta: %f\n", p_timeDelta);
	this->durationElapsed += p_timeDelta;
	
	// change scene if splash screen should end
	if(this->durationElapsed >= this->durationEnd)
		sceneManager.switchFocusTo(this->nextSceneId);
}

void SceneSplashScreen::drawUpper(RenderWindow& p_renderWindow) {
	// eprintf("DrawU\n");
	p_renderWindow.clear(C2D_Color32(0,0,0,0));
	p_renderWindow.draw(this->splashImageEntity);
}
void SceneSplashScreen::drawLower(RenderWindow& p_renderWindow) {
	// ((void)0);
	p_renderWindow.clear(C2D_Color32(0,0,0,0));
	// eprintf("DrawL\n");
} // no-op