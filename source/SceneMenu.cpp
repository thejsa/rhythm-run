#include "SceneMenu.hpp"

SceneMenu::SceneMenu(SceneManager& a_sceneManager,
	AudioManager& a_audioManager)
:sceneManager(a_sceneManager), audioManager(a_audioManager)
{};

void SceneMenu::onCreate()
{
	// Load sprite sheet
	eprintf("OnCreate\n");

	spriteSheet = C2D_SpriteSheetLoad("romfs:/gfx/SceneMenu.t3x");
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
		spriteSheet, 1, // which sheet & image to load
		0.5f, 0.5f, // sprite's origin
		1.0f, 1.0f, // scale
		0.0f //rotation
	);

	// Play audio
	// soloud.init();
	// sample.load("romfs:/sample.wav");
	int error = 0;

	char bgmPath[128];
	snprintf(bgmPath, std::extent_v<decltype(bgmPath)>, "romfs:/tracks/newspapers_for_magicians.opus");
	eprintf("opening %s\n", bgmPath);

	opusFile = std::shared_ptr<OggOpusFile>(op_open_file (bgmPath, &error), op_free);

	if (error)
	{
		eprintf("Failed to open file! error: %d\n", error);
		opusFile = nullptr;
	} else {
		audioId = audioManager.addFile(opusFile);
		eprintf("opus file: %x -> id %d\n", opusFile, audioId);
		// audioId = 1337;
	}

	eprintf("Fini\n");
}

void SceneMenu::onFocus()
{
	// eprintf("Focus\n");
	// Reset duration timer
	// durationElapsed = 0.0f;

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

void SceneMenu::onBlur()
{
	if(opusFile) {
		audioManager.pause();
		eprintf("pause audio\n");
	}
}

void SceneMenu::onDestroy() {
	// soloud.deinit();
	// if(opusFile) op_free(opusFile);
	// smart pointer means we don't need to?
	eprintf("OnDestroy\n");
	audioManager.stop();
}

// void SceneMenu::setNextSceneId(unsigned int a_id)
// {
// 	eprintf("%u\n", a_id);
// 	nextSceneId = a_id;
// }

void SceneMenu::processInput()
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

void SceneMenu::update(float a_timeDelta)
{
	((void)0);
	// eprintf("Update, delta: %f\n", a_timeDelta);
	// durationElapsed += a_timeDelta;
	
	// change scene if splash screen should end
	// if(durationElapsed >= durationEnd)
	// 	sceneManager.switchFocusTo(nextSceneId);
	// if(durationElapsed >= durationEnd) {
		// eprintf("durationElapsed: + %.2f s\n", durationElapsed);
		// durationElapsed = 0;
		// sceneManager.switchFocusTo(nextSceneId);
	// }
}

void SceneMenu::drawUpper(RenderWindow& a_renderWindow) {
	// eprintf("DrawU\n");
	a_renderWindow.clear(C2D_Color32(0,0,0,0));
	a_renderWindow.draw(splashImageEntity);
}
void SceneMenu::drawLower(RenderWindow& a_renderWindow) {
	// ((void)0);
	a_renderWindow.clear(C2D_Color32(0,0,0,0));
	// eprintf("DrawL\n");
} // no-op