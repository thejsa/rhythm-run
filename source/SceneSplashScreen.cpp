#include "SceneSplashScreen.hpp"

SceneSplashScreen::SceneSplashScreen(SceneManager& a_sceneManager,
	AudioManager& a_audioManager, unsigned int a_spriteIndex)
:sceneManager(a_sceneManager), audioManager(a_audioManager), splashImageIndex(a_spriteIndex),
	nextSceneId(0), durationEnd(3.0f), durationElapsed(0.0f)
{};

void SceneSplashScreen::onCreate()
{
	// Load sprite sheet
	eprintf("OnCreate\n");

	spriteSheet = C2D_SpriteSheetLoad("romfs:/gfx/splash.t3x");
	if (!spriteSheet) {
		eprintf("failed to load sprite sheet\n");
		// justSpin();
		svcBreak(USERBREAK_PANIC);
	}
	
	// Create sprite
	// float scale;
	// if(splashImageIndex == 2) {
	// 	scale = 4.0f;
	// } else {
	// 	scale = 1.0f;
	// }
	splashImageEntity = std::make_shared<Entity>(200, 120, // centre of screen
		spriteSheet, splashImageIndex, // which sheet & image to load
		0.5f, 0.5f, // sprite's origin
		1.0f, 1.0f, // scale
		0.0f //rotation
	);

	// Play audio
	// soloud.init();
	// sample.load("romfs:/sample.wav");
	int error = 0;
	opusFile = std::shared_ptr<OggOpusFile>(op_open_file ("romfs:/sample.opus", &error), op_free);

	if (error)
	{
		eprintf("Failed to open file! error: %d\n", error);
		opusFile = nullptr;
	} else {
		audioId = audioManager.addFile(opusFile);
		// eprintf("opus file: %x -> id %d\n", opusFile, audioId);
		// audioId = 1337;
	}

	eprintf("Fini\n");
}

void SceneSplashScreen::onFocus()
{
	// eprintf("Focus\n");
	// Reset duration timer
	durationElapsed = 0.0f;

	if(opusFile) {
		eprintf("switch to audio %d\n", audioId);
		int err = audioManager.switchFileTo(audioId);
		if(err != 0) {
			eprintf("switchFileTo returned error: %d\n", err);
		}
		audioManager.play();
	}
	
	// sample.load("romfs:/sample.wav");
	// soloud.play(sample);
}

void SceneSplashScreen::onBlur()
{
	if(opusFile) {
		audioManager.pause();
		eprintf("pause audio\n");
	}
}

void SceneSplashScreen::onDestroy() {
	// soloud.deinit();
	// if(opusFile) op_free(opusFile);
	// smart pointer means we don't need to?
	eprintf("OnDestroy\n");
	audioManager.stop();
}

void SceneSplashScreen::setNextSceneId(unsigned int a_id)
{
	eprintf("%u\n", a_id);
	nextSceneId = a_id;
}

void SceneSplashScreen::processInput()
{
	// Quit if APT says we should, or if user presses the START key
	u32 kDown = hidKeysDown();
	if(kDown & KEY_A) {
		eprintf("keyA\n");
		audioManager.pause();
	};
	if(kDown & KEY_B) {
		eprintf("keyB\n");
		audioManager.unpause();
	};
	if(kDown & KEY_Y) {
		eprintf("keyY\n");
		audioManager.togglePause();
	};
	if(kDown & KEY_L) {
		eprintf("keyL\n");
		if(!audioManager.isStopped()) {
			audioManager.stop();
		} else {
			eprintf("already stopped!\n");
		}
	};
	if(kDown & KEY_R) {
		eprintf("keyR\n");
		if(audioManager.isStopped()) {
			audioManager.play();
		} else {
			eprintf("already playing!\n");
		}
	};
}

void SceneSplashScreen::update(float a_timeDelta)
{
	// eprintf("Update, delta: %f\n", a_timeDelta);
	durationElapsed += a_timeDelta;
	
	// change scene if splash screen should end
	// if(durationElapsed >= durationEnd)
	// 	sceneManager.switchFocusTo(nextSceneId);
	if(durationElapsed >= durationEnd) {
		eprintf("durationElapsed: + %.2f s\n", durationElapsed);
		sceneManager.switchFocusTo(nextSceneId);
		durationElapsed = 0;
	}
}

void SceneSplashScreen::drawUpper(RenderWindow& a_renderWindow) {
	// eprintf("DrawU\n");
	a_renderWindow.clear(C2D_Color32(0,0,0,0));
	a_renderWindow.draw(splashImageEntity);
}
void SceneSplashScreen::drawLower(RenderWindow& a_renderWindow) {
	// ((void)0);
	a_renderWindow.clear(C2D_Color32(0,0,0,0));
	// eprintf("DrawL\n");
} // no-op