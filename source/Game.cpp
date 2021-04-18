/*
 * Rhythm Run for Nintendo 3DS
 * Lauren Kelly, 2020
 */

#include "debug.hpp"

#include "Entity.hpp"
#include "Game.hpp"
#include "Label.hpp"

#include "SceneGameplay.hpp"
#include "SceneMenu.hpp"
#include "SceneSplashScreen.hpp"

#include <3ds.h>
#include <memory>

#include <citro2d.h>

Game::Game()
    : upperScreen(GFX_TOP, GFX_LEFT)
    , lowerScreen(GFX_BOTTOM, GFX_LEFT)
{
    // Init clock
    // PLATFORM SPECIFIC
    lastTime = osGetTime(); // milliseconds
    timeDelta = 0.0f;

    // these are automatic?
    // sceneManager = SceneManager();
    // audioManager = AudioManager();

    // Init scenes

    std::shared_ptr<SceneSplashScreen> splashScene1 = std::make_shared<SceneSplashScreen>(
        sceneManager,
        audioManager,
        0 // sprite index
    );
    std::shared_ptr<SceneSplashScreen> splashScene2 = std::make_shared<SceneSplashScreen>(
        sceneManager,
        audioManager,
        2 // sprite index
    );
    std::shared_ptr<SceneGameplay> gameplayScene = std::make_shared<SceneGameplay>(
        sceneManager,
        audioManager,
        // "newspapers_for_magicians" // track
        "moonlight_sonata" // track
    );

    eprintf("adding scenes to manager\n");

    unsigned int splashScene1Id = sceneManager.addScene(splashScene1);
    unsigned int splashScene2Id = sceneManager.addScene(splashScene2);
    unsigned int gameplaySceneId = sceneManager.addScene(gameplayScene);

    std::shared_ptr<SceneMenu> theMenu = std::make_shared<SceneMenu>(
        sceneManager,
        audioManager,
        gameplaySceneId,
        splashScene1Id);

    unsigned int menuSceneId = sceneManager.addScene(theMenu);

    // eprintf("setting nextSceneIds %u->%u %u->%u %u->%u\n", splashScene1Id, splashScene2Id, splashScene2Id, splashScene3Id, splashScene3Id, splashScene1Id);

    // just cycle the splash screens until we implement more fun stuff
    splashScene1->setNextSceneId(splashScene2Id);
    splashScene2->setNextSceneId(menuSceneId);
    gameplayScene->setNextSceneId(menuSceneId);
    // splashScene1->setNextSceneId(splashScene2Id);
    // splashScene2->setNextSceneId(splashScene3Id);
    // splashScene3->setNextSceneId(splashScene1Id);

    eprintf("Begin by switching focus to menu scene, id: %u\n", menuSceneId);
    sceneManager.switchFocusTo(menuSceneId);
}

void Game::stepTimer()
{
    u64 currentTime = osGetTime(); // ms
    timeDelta = (currentTime - lastTime) / 1000.0f; // seconds
    // eprintf("current: %llu, last: %llu, delta: %f\n", currentTime, lastTime, timeDelta);
    lastTime = currentTime;
    // return timeDelta;
}

void Game::processInput()
{
    // Respond to user input
    hidScanInput();

    if (hidKeysDown() & KEY_START) {
        eprintf("keyStart -- quit flag!\n");
        // signal sceneManager to shutdown
        sceneManager.shutdown();
    }

    sceneManager.processInput();

    // eprintf("update end\n");
};

void Game::update()
{
    // ((void)0);
    stepTimer();

    sceneManager.update(timeDelta);

    // eprintf("update end\n");
};

void Game::lateUpdate()
{
    sceneManager.lateUpdate(timeDelta);
};

void Game::draw()
{
    C3D_FrameBegin(C3D_FRAME_SYNCDRAW);

    sceneManager.draw(upperScreen, lowerScreen);

    C3D_FrameEnd(0);
}