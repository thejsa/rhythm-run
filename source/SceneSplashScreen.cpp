#include "SceneSplashScreen.hpp"

SceneSplashScreen::SceneSplashScreen(SceneManager& a_sceneManager,
	AudioManager& a_audioManager, unsigned int a_spriteIndex)
:sceneManager(a_sceneManager), audioManager(a_audioManager), splashImageIndex(a_spriteIndex),
	nextSceneId(0), durationEnd(1.0f), durationElapsed(0.0f)
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
		eprintf("opus file: %x -> id %d\n", this->opusFile, this->audioId);
		// this->audioId = 1337;
	}

	eprintf("Fini\n");
}

void SceneSplashScreen::onFocus()
{
	// eprintf("Focus\n");
	// Reset duration timer
	this->durationElapsed = 0.0f;

	if(this->opusFile) {
		eprintf("switch to audio %d\n", this->audioId);
		int err = this->audioManager.switchFileTo(this->audioId);
		if(err != 0) {
			eprintf("switchFileTo returned error: %d\n", err);
		}
		this->audioManager.play();
	}
	
	// sample.load("romfs:/sample.wav");
	// soloud.play(sample);
}

void SceneSplashScreen::onBlur()
{
	if(this->opusFile) {
		this->audioManager.pause();
		eprintf("pause audio\n");
	}
}

void SceneSplashScreen::onDestroy() {
	// soloud.deinit();
	// if(this->opusFile) op_free(this->opusFile);
	// smart pointer means we don't need to?
	eprintf("OnDestroy\n");
	this->audioManager.stop();
}

void SceneSplashScreen::setNextSceneId(unsigned int a_id)
{
	eprintf("%u\n", a_id);
	this->nextSceneId = a_id;
}

void SceneSplashScreen::processInput()
{
	// Quit if APT says we should, or if user presses the START key
	u32 kDown = hidKeysDown();
	if(kDown & KEY_A) {
		eprintf("keyA\n");
		this->audioManager.pause();
	};
	if(kDown & KEY_B) {
		eprintf("keyB\n");
		this->audioManager.unpause();
	};
	if(kDown & KEY_Y) {
		eprintf("keyY\n");
		this->audioManager.togglePause();
	};
	if(kDown & KEY_L) {
		eprintf("keyL\n");
		if(!this->audioManager.isStopped()) {
			this->audioManager.stop();
		} else {
			eprintf("already stopped!\n");
		}
	};
	if(kDown & KEY_R) {
		eprintf("keyR\n");
		if(this->audioManager.isStopped()) {
			this->audioManager.play();
		} else {
			eprintf("already playing!\n");
		}
	};
}

void SceneSplashScreen::update(float a_timeDelta)
{
	// eprintf("Update, delta: %f\n", a_timeDelta);
	this->durationElapsed += a_timeDelta;
	
	// change scene if splash screen should end
	// if(this->durationElapsed >= this->durationEnd)
	// 	sceneManager.switchFocusTo(this->nextSceneId);
	if(this->durationElapsed >= this->durationEnd) {
		eprintf("durationElapsed: + %.2f s\n", this->durationElapsed);
		sceneManager.switchFocusTo(this->nextSceneId);
		this->durationElapsed = 0;
	}
}

void SceneSplashScreen::drawUpper(RenderWindow& a_renderWindow) {
	// eprintf("DrawU\n");
	a_renderWindow.clear(C2D_Color32(0,0,0,0));
	a_renderWindow.draw(this->splashImageEntity);
}
void SceneSplashScreen::drawLower(RenderWindow& a_renderWindow) {
	// ((void)0);
	a_renderWindow.clear(C2D_Color32(0,0,0,0));
	// eprintf("DrawL\n");
} // no-op