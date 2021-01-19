#include "SceneGameplay.hpp"

SceneGameplay::SceneGameplay(SceneManager& p_sceneManager,
	AudioManager& p_audioManager, const char* p_trackName)
:sceneManager(p_sceneManager), audioManager(p_audioManager), trackName(p_trackName),
	nextSceneId(0), durationEnd(60.0f), durationElapsed(0.0f)
{};

void SceneGameplay::onCreate()
{
	// Load sprite sheet
	eprintf("OnCreate\n");

	this->spriteSheet = C2D_SpriteSheetLoad("romfs:/gfx/sprites.t3x");
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
		this->spriteSheet, 1, // which sheet & image to load
		0.5f, 0.5f, // sprite's origin
		1.0f, 1.0f, // scale
		0.0f //rotation
	);

	// Play audio
	// soloud.init();
	// sample.load("romfs:/sample.wav");
	int error = 0;

	char bgmPath[128];
	snprintf(bgmPath, std::extent_v<decltype(bgmPath)>, "romfs:/tracks/%s.opus", this->trackName);
	eprintf("opening %s\n", bgmPath);

	this->opusFile = std::shared_ptr<OggOpusFile>(op_open_file (bgmPath, &error), op_free);

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

void SceneGameplay::onFocus()
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

void SceneGameplay::onBlur()
{
	if(this->opusFile) {
		this->audioManager.pause();
		eprintf("pause audio\n");
	}
}

void SceneGameplay::onDestroy() {
	// soloud.deinit();
	// if(this->opusFile) op_free(this->opusFile);
	// smart pointer means we don't need to?
	eprintf("OnDestroy\n");
	this->audioManager.stop();
}

void SceneGameplay::setNextSceneId(unsigned int p_id)
{
	eprintf("%u\n", p_id);
	this->nextSceneId = p_id;
}

void SceneGameplay::processInput()
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

void SceneGameplay::update(float p_timeDelta)
{
	// eprintf("Update, delta: %f\n", p_timeDelta);
	this->durationElapsed += p_timeDelta;
	
	// change scene if splash screen should end
	// if(this->durationElapsed >= this->durationEnd)
	// 	sceneManager.switchFocusTo(this->nextSceneId);
	if(this->durationElapsed >= this->durationEnd) {
		eprintf("durationElapsed: + %.2f s\n", this->durationElapsed);
		this->durationElapsed = 0;
		sceneManager.switchFocusTo(this->nextSceneId);
	}
}

void SceneGameplay::drawUpper(RenderWindow& p_renderWindow) {
	// eprintf("DrawU\n");
	p_renderWindow.clear(C2D_Color32(0,0,0,0));
	p_renderWindow.draw(this->splashImageEntity);
}
void SceneGameplay::drawLower(RenderWindow& p_renderWindow) {
	// ((void)0);
	p_renderWindow.clear(C2D_Color32(0,0,0,0));
	// eprintf("DrawL\n");
} // no-op