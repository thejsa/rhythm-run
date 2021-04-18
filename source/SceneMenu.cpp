#include "SceneMenu.hpp"

SceneMenu::SceneMenu(SceneManager& a_sceneManager,
    AudioManager& a_audioManager,
    unsigned int a_gameplaySceneId,
    unsigned int a_splashSceneId)
    : sceneManager(a_sceneManager)
    , audioManager(a_audioManager)
    , gameplaySceneId(a_gameplaySceneId)
    , splashSceneId(a_splashSceneId) {};

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
        spriteSheet, 0, // which sheet & image to load
        0.5f, 0.5f, // sprite's origin
        1.0f, 1.0f, // scale
        0.0f //rotation
    );

    // gameplay scene button
    button1 = std::make_shared<LabelledButton>(40, 48, // x,y
        spriteSheet, 1, 2, // which sheet & image to load
        0.0f, 0.0f, // sprite's origin
        "Gameplay Test",
        1.0f, 1.0f, // scale
        0.0f, //rotation
        C2D_Color32(0xff, 0xff, 0xff, 0xff));
    // button1 = std::make_shared<Button>(40, 48, // x,y
    // 	spriteSheet, 1, // which sheet & image to load
    // 	0.0f, 0.0f, // sprite's origin
    // 	1.0f, 1.0f, // scale
    // 	0.0f, //rotation
    // 	C2D_Color32(0xff, 0xff, 0xff, 0xff)
    // );
    // button1Label = std::make_shared<Label>("Gameplay Test", 160, 48+24,
    // 	C2D_AtBaseline | C2D_AlignCenter,
    // 	0.75f, 0.75f, // font size
    // 	C2D_Color32(0x00, 0xff, 0x00, 0xff),
    // 	0
    // );

    // splash scene button
    button2 = std::make_shared<LabelledButton>(40, 104, // x,y
        spriteSheet, 1, 2, // which sheet & image to load
        0.0f, 0.0f, // sprite's origin
        "Splash Screen",
        1.0f, 1.0f, // scale
        0.0f, //rotation
        C2D_Color32(0xff, 0xff, 0xff, 0xff));
    // button2Label = std::make_shared<Label>("Splash Screen", 160, 104+24,
    // 	C2D_AtBaseline | C2D_AlignCenter,
    // 	0.75f, 0.75f, // font size
    // 	C2D_Color32(0xff, 0xff, 0xff, 0xff),
    // 	0
    // );

    // exit button
    button3 = std::make_shared<LabelledButton>(40, 160, // x,y
        spriteSheet, 1, 2, // which sheet & image to load
        0.0f, 0.0f, // sprite's origin
        "Quit Game",
        0.5f, 0.75f, // scale
        0.0f, //rotation
        C2D_Color32(0xff, 0xff, 0xff, 0xff));
    // button3Label = std::make_shared<Label>("Exit", 160, 160+24,
    // 	C2D_AtBaseline | C2D_AlignCenter,
    // 	0.75f, 0.75f, // font size
    // 	C2D_Color32(0xff, 0xff, 0xff, 0xff),
    // 	0
    // );

    // Play audio
    // soloud.init();
    // sample.load("romfs:/sample.wav");
    // int error = 0;

    // char bgmPath[128];
    // snprintf(bgmPath, std::extent_v<decltype(bgmPath)>, "romfs:/tracks/newspapers_for_magicians.opus");
    // eprintf("opening %s\n", bgmPath);

    // opusFile = std::shared_ptr<OggOpusFile>(op_open_file (bgmPath, &error), op_free);

    // if (error)
    // {
    // 	eprintf("Failed to open file! error: %d\n", error);
    // 	opusFile = nullptr;
    // } else {
    // 	audioId = audioManager.addFile(opusFile);
    // 	// eprintf("opus file: %x -> id %d\n", opusFile, audioId);
    // 	// audioId = 1337;
    // }
    opusFile = nullptr; // no music

    eprintf("Fini\n");
}

void SceneMenu::onFocus()
{
    // eprintf("Focus\n");
    // Reset duration timer
    // durationElapsed = 0.0f;

    if (opusFile) {
        eprintf("switch to audio %d\n", audioId);
        int err = audioManager.switchFileTo(audioId);
        if (err != 0) {
            eprintf("switchFileTo returned error: %d\n", err);
        }
        audioManager.play();
    }

    // sample.load("romfs:/sample.wav");
    // soloud.play(sample);
}

void SceneMenu::onBlur()
{
    if (opusFile) {
        audioManager.pause();
        eprintf("pause audio\n");
    }
}

void SceneMenu::onDestroy()
{
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
    if (kDown & KEY_A) {
        eprintf("keyA\n");
        audioManager.pause();
    };
    if (kDown & KEY_B) {
        eprintf("keyB\n");
        audioManager.unpause();
    };
    if (kDown & KEY_Y) {
        eprintf("keyY\n");
        audioManager.togglePause();
    };
    if (kDown & KEY_L) {
        eprintf("keyL\n");
        if (!audioManager.isStopped()) {
            audioManager.stop();
        } else {
            eprintf("already stopped!\n");
        }
    };
    if (kDown & KEY_R) {
        eprintf("keyR\n");
        if (audioManager.isStopped()) {
            audioManager.play();
        } else {
            eprintf("already playing!\n");
        }
    };

    // read touch
    touchPosition tpos;
    hidTouchRead(&tpos);
    Point touchPoint = {
        tpos.px * 1.0f,
        tpos.py * 1.0f
    };
    Rectangle touchRect { touchPoint, touchPoint };

    Rectangle button1bbox = button1->getRect();
    Rectangle button2bbox = button2->getRect();
    Rectangle button3bbox = button3->getRect();

    // button 1- gameplay
    if (button1bbox.doesIntersect(touchRect) && !button1->getPressed()) {
        // printf("b1 pressed\n");
        button1->setPressed(true);
        // button1Label->move(2.0f, 2.0f);
    } else if (tpos.px == 0 && tpos.py == 0 && button1->getPressed()) {
        // printf("b1 button pressed&released!\n");
        button1->setPressed(false);
        // button1Label->move(-2.0f, -2.0f);

        sceneManager.switchFocusTo(gameplaySceneId);
    } else {
        // printf("button not pressed");
    }

    // button 2- splash
    if (button2bbox.doesIntersect(touchRect) && !button2->getPressed()) {
        // printf("b2 pressed\n");
        button2->setPressed(true);
        // button2Label->move(2.0f, 2.0f);
    } else if (tpos.px == 0 && tpos.py == 0 && button2->getPressed()) {
        // printf("b2 button pressed&released!\n");
        button2->setPressed(false);
        // button2Label->move(-2.0f, -2.0f);

        sceneManager.switchFocusTo(splashSceneId);
    } else {
        // printf("button not pressed");
    }

    // button 3- quit
    if (button3bbox.doesIntersect(touchRect) && !button3->getPressed()) {
        // printf("b3 pressed\n");
        button3->setPressed(true);
        // button3Label->move(2.0f, 2.0f);
    } else if (tpos.px == 0 && tpos.py == 0 && button3->getPressed()) {
        // printf("b3 button pressed&released!\n");
        button3->setPressed(false);
        // button3Label->move(-2.0f, -2.0f);

        // can shutdown
        sceneManager.shutdown();
    } else {
        // printf("button not pressed");
    }

    // printf("bbox (%.0f, %.0f), (%.0f, %.0f)\n", bbox.topLeft.x, bbox.topLeft.y, bbox.lowerRight.x, bbox.lowerRight.y);
    // printf("touch (%.0f, %.0f), (%.0f, %.0f)\n", touchRect.topLeft.x, touchRect.topLeft.y, touchRect.lowerRight.x, touchRect.lowerRight.y);
    // // eprintf("%.0f, %.0f\n", tpos.px * 1.0f, tpos.py * 1.0f);
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

void SceneMenu::drawUpper(RenderWindow& a_renderWindow)
{
    // eprintf("DrawU\n");
    a_renderWindow.clear(C2D_Color32(0, 0, 0, 0));
    a_renderWindow.draw(splashImageEntity);
}
void SceneMenu::drawLower(RenderWindow& a_renderWindow)
{
    // ((void)0);
    a_renderWindow.clear(C2D_Color32(0xDD, 0xFF, 0xFF, 0xFF));

    a_renderWindow.draw(button1);
    // a_renderWindow.draw(button1Label);
    a_renderWindow.draw(button2);
    // a_renderWindow.draw(button2Label);
    a_renderWindow.draw(button3);
    // a_renderWindow.draw(button3Label);
    // eprintf("DrawL\n");
} // no-op