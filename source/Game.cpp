/*
 * Rhythm Run for Nintendo 3DS
 * Lauren Kelly, 2020
 */

#include "debug.hpp"

#include "Game.hpp"
#include "Label.hpp"
#include "Entity.hpp"

#include "SceneSplashScreen.hpp"
#include "SceneGameplay.hpp"

#include <3ds.h>
#include <memory>

#include <citro2d.h>

Game::Game()
:upperScreen(GFX_TOP, GFX_LEFT), lowerScreen(GFX_BOTTOM, GFX_LEFT), quitFlag(false)
{
	// Init clock
	// PLATFORM SPECIFIC
	lastTime = osGetTime(); // milliseconds
	timeDelta = 0.0f;

	// these are automatic?
	// sceneManager = SceneManager();
	// audioManager = AudioManager();
	
	// Init test scene
	std::shared_ptr<SceneSplashScreen> splashScene1 = std::make_shared<SceneSplashScreen>(
		sceneManager,
		audioManager,
		0 // sprite index
	);
	std::shared_ptr<SceneGameplay> gameplayScene = std::make_shared<SceneGameplay>(
		sceneManager,
		audioManager,
		"newspapers_for_magicians" // track
	);
	// std::shared_ptr<SceneSplashScreen> splashScene2 = std::make_shared<SceneSplashScreen>(
	// 	sceneManager,
	// 	audioManager,
	// 	1 // sprite index
	// );
	// std::shared_ptr<SceneSplashScreen> splashScene3 = std::make_shared<SceneSplashScreen>(
	// 	sceneManager,
	// 	audioManager,
	// 	2 // sprite index
	// );
	
	eprintf("adding scenes to manager %x\n", sceneManager);

	unsigned int splashScene1Id = sceneManager.addScene(splashScene1);
	unsigned int gameplaySceneId = sceneManager.addScene(gameplayScene);
	// unsigned int splashScene2Id = sceneManager.addScene(splashScene2);
	// unsigned int splashScene3Id = sceneManager.addScene(splashScene3);
	
	// eprintf("setting nextSceneIds %u->%u %u->%u %u->%u\n", splashScene1Id, splashScene2Id, splashScene2Id, splashScene3Id, splashScene3Id, splashScene1Id);

	// just cycle the splash screens until we implement more fun stuff
	splashScene1->setNextSceneId(gameplaySceneId);
	gameplayScene->setNextSceneId(splashScene1Id);
	// splashScene1->setNextSceneId(splashScene2Id);
	// splashScene2->setNextSceneId(splashScene3Id);
	// splashScene3->setNextSceneId(splashScene1Id);

	eprintf("Begin by switching focus to scene %u\n", splashScene1Id);
	sceneManager.switchFocusTo(splashScene1Id);
}

void Game::stepTimer() {
	u64 currentTime = osGetTime(); // ms
	timeDelta = (currentTime - lastTime) / 1000.0f; // seconds
	// eprintf("current: %llu, last: %llu, delta: %f\n", currentTime, lastTime, timeDelta);
	lastTime = currentTime;
	// return timeDelta;
}

void Game::processInput() {
	// Respond to user input
	hidScanInput();
	
	if(hidKeysDown() & KEY_START) {
		eprintf("keyStart -- quit flag!\n");
		quitFlag = true; // return to hbmenu
	}
	
	sceneManager.processInput();

	// eprintf("update end\n");
};

void Game::update() {
	// ((void)0);
	stepTimer();
	
	sceneManager.update(timeDelta);

	// eprintf("update end\n");
};

void Game::lateUpdate() { ((void)0); };

void Game::drawUpper() {
	// eprintf("BeginDrawU\n");
	upperScreen.beginDraw();
	sceneManager.drawUpper(upperScreen);
	upperScreen.endDraw();
	// eprintf("End\n");
};

void Game::drawLower() {
	// eprintf("BeginDrawL\n");
	lowerScreen.beginDraw();
	sceneManager.drawLower(lowerScreen);
	lowerScreen.endDraw();
	// eprintf("End\n");
};