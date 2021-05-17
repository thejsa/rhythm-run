#include "SceneGameplay.hpp"

SceneGameplay::SceneGameplay(SceneManager& a_sceneManager,
    AudioManager& a_audioManager, const char* a_trackName)
    : sceneManager(a_sceneManager)
    , audioManager(a_audioManager)
    , trackName(a_trackName)
    , nextSceneId(0)
    , durationEnd(1 / 30.0f)
    , durationElapsed(0.0f) {};

void SceneGameplay::onCreate()
{
    // Load sprite sheet
    eprintf("OnCreate\n");

    spriteSheet = C2D_SpriteSheetLoad("romfs:/gfx/sprites.t3x");
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

    debugLabel = std::make_shared<Label>("Hello, world!", 8, 24,
        C2D_AtBaseline | C2D_AlignLeft,
        0.5f, 0.5f, // font size
        C2D_Color32(0xb0, 0x0b, 0x69, 0xff),
        0);

    // position at bottom left of screen
    myPlayer = std::make_shared<Player>(std::make_unique<Entity>(
        200, 120,
        spriteSheet, 2,
        0.0f, 1.0f,
        1.0f, 1.0f,
        0.0f));

    // create a platform entity
    platform1Entity = std::make_shared<Entity>(200, 120, // centre of screen
        spriteSheet, 3, // which sheet & image to load
        // 0.5f, 0.5f, // sprite's origin
        0.0f, 1.0f, // sprite's origin -- bottom left
        0.25f, 0.25f, // scale
        0.0f //rotation
    );
    // copy the platform
    platform2Entity = std::make_shared<Entity>(*platform1Entity);
    platform3Entity = std::make_shared<Entity>(*platform1Entity);
    platform4Entity = std::make_shared<Entity>(*platform1Entity);

    // position the platforms
    platform1Entity->setPosition(72, 216);
    platform2Entity->setPosition(120, 216);
    platform3Entity->setPosition(168, 216);
    platform4Entity->setPosition(216, 216);

    // Play audio
    // soloud.init();
    // sample.load("romfs:/sample.wav");
    int error = 0;

    char bgmPath[128];
    snprintf(bgmPath, std::extent_v<decltype(bgmPath)>, "romfs:/tracks/%s.opus", trackName);
    eprintf("opening %s\n", bgmPath);

    opusFile = std::shared_ptr<OggOpusFile>(op_open_file(bgmPath, &error), op_free);

    if (error) {
        eprintf("Failed to open file! error: %d\n", error);
        opusFile = nullptr;
    } else {
        audioId = audioManager.addFile(opusFile);
        // eprintf("opus file: %x -> id %d\n", opusFile, audioId);
        // audioId = 1337;
    }

    eprintf("Fini\n");
}

void SceneGameplay::onFocus()
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

void SceneGameplay::onBlur()
{
    if (opusFile) {
        audioManager.pause();
        eprintf("pause audio\n");
    }
}

void SceneGameplay::onDestroy()
{
    // soloud.deinit();
    // if(opusFile) op_free(opusFile);
    // smart pointer means we don't need to?
    eprintf("OnDestroy\n");
    audioManager.stop();
}

void SceneGameplay::setNextSceneId(unsigned int a_id)
{
    eprintf("%u\n", a_id);
    nextSceneId = a_id;
}

void SceneGameplay::processInput()
{
    // Quit if APT says we should, or if user presses the START key
    u32 kDown = hidKeysDown();
    // u32 kHeld = hidKeysHeld();
    if (kDown & KEY_A) {
        eprintf("keyA\n");
        // audioManager.pause();
    };
    if (kDown & KEY_B) {
        eprintf("keyB\n");
        // audioManager.unpause();
    };
    if (kDown & KEY_Y) {
        eprintf("keyY pause audio toggle\n");
        audioManager.togglePause();
    };
    if (kDown & KEY_L) {
        eprintf("keyL stop audio\n");
        if (!audioManager.isStopped()) {
            audioManager.stop();
        } else {
            eprintf("already stopped!\n");
        }
    };
    if (kDown & KEY_R) {
        eprintf("keyR un-stop audio/begin play\n");
        if (audioManager.isStopped()) {
            audioManager.play();
        } else {
            eprintf("already playing!\n");
        }
    };

    if (kDown & KEY_SELECT) {
        sceneManager.switchFocusTo(nextSceneId);
    }
}

void SceneGameplay::update(float a_timeDelta)
{
    static char debugLabelBuf[256];
    // if we should shutdown, cleanup
    if (sceneManager.shouldShutdown()) {
        audioManager.stop();
        return;
    }

    // eprintf("Update, delta: %f\n", a_timeDelta);
    durationElapsed += a_timeDelta;

    // update player
    myPlayer->update(a_timeDelta);

    // update debug label
    const char* currentState;
    switch (myPlayer->currentState) {
    case Player::State::Jump:
        currentState = "Jump";
        break;
    case Player::State::DoubleJump:
        currentState = "Jump (double)";
        break;
    case Player::State::Stand:
        currentState = "Standing";
        break;
    case Player::State::Walk:
        currentState = "Walk";
        break;
    case Player::State::GrabLedge:
        currentState = "Grabbing ledge";
        break;
    default:
        currentState = "Unknown";
    }

    int debugLabelSize = sprintf(debugLabelBuf, "v: (%.02f, %.02f)\na: (%.02f, %.02f)\np: (%.02f, %.02f)\nstate: %s\nisOnGround?: %d",
        myPlayer->speed.x,
        myPlayer->speed.y,
        myPlayer->accel.x,
        myPlayer->accel.y,
        myPlayer->position.x,
        myPlayer->position.y,
        currentState,
        myPlayer->isOnGround);
    debugLabel->setText(debugLabelBuf, debugLabelSize);

    // change scene if splash screen should end
    // if(durationElapsed >= durationEnd)
    // 	sceneManager.switchFocusTo(nextSceneId);
}

void SceneGameplay::draw(RenderWindow& a_renderWindowUpper, RenderWindow& a_renderWindowLower)
{
    a_renderWindowUpper.beginDraw();
    a_renderWindowUpper.clear(C2D_Color32(255, 255, 255, 255));
    a_renderWindowUpper.draw(myPlayer);
    a_renderWindowUpper.draw(platform1Entity);
    a_renderWindowUpper.draw(platform2Entity);
    a_renderWindowUpper.draw(platform3Entity);
    a_renderWindowUpper.draw(platform4Entity);

    a_renderWindowLower.beginDraw();
    a_renderWindowLower.clear(C2D_Color32(255, 255, 255, 255));
    a_renderWindowLower.draw(debugLabel);
}