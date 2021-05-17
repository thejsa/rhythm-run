#include "SceneGameplay.hpp"

SceneGameplay::SceneGameplay(SceneManager& a_sceneManager,
    AudioManager& a_audioManager, const char* a_trackName)
    : sceneManager(a_sceneManager)
    , audioManager(a_audioManager)
    , trackName(a_trackName)
    , nextSceneId(0)
    , durationEnd(1 / 30.0f)
    , durationElapsed(0.0f)
    , isJumping(false)
    , shouldRedraw(true)
    , velocityX(0)
    , velocityY(0)
    , accelX(0)
    , accelY(0) {};

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

    playerEntity = std::make_shared<Entity>(200, 120, // centre of screen
        spriteSheet, 2, // which sheet & image to load
        // 0.5f, 0.5f, // sprite's origin
        0.0f, 1.0f, // sprite's origin -- bottom left
        1.0f, 1.0f, // scale
        0.0f //rotation
    );

    myPlayer = std::make_shared<Player>(std::make_unique<Entity>(
        200, 120,
        spriteSheet, 2,
        0.0f, 1.0f,
        1.0f, 1.0f,
        0.0f));

    // reposition to bottom left of screen
    playerEntity->setPosition(0, 240);

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
    static const float fGravity = -9.81f;

    // Quit if APT says we should, or if user presses the START key
    u32 kDown = hidKeysDown();
    u32 kHeld = hidKeysHeld();
    if (kDown & KEY_A) {
        eprintf("keyA\n");
        // if(!isJumping) {
        isJumping = true;
        velocityY = -40.0f * fGravity;
        accelY = 30.0f * fGravity * 2.5f;
        // }
        // audioManager.pause();
    };
    if (kDown & KEY_B) {
        eprintf("keyB\n");
        // audioManager.unpause();
        eprintf("vel: %.2f, %.2f; acc: %.2f, %.2f; isJumping: %s\n",
            velocityX, velocityY,
            accelX, accelY,
            (isJumping ? "true" : "false"));
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

    if (kHeld & KEY_RIGHT) {
        // eprintf("keyRight\n");
        playerEntity->move(4, 0);
    }
    if (kHeld & KEY_LEFT) {
        // eprintf("keyLeft\n");
        playerEntity->move(-4, 0);
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

    // calculate new velocity
    velocityX = std::clamp<float>(
        velocityX + (accelX * a_timeDelta),
        -10000,
        10000);
    velocityY = std::clamp<float>(
        velocityY + (accelY * a_timeDelta),
        -10000,
        10000);

    // calculate new position
    float newX = playerEntity->getX() + (velocityX * a_timeDelta);
    float newY = playerEntity->getY() - (velocityY * a_timeDelta);

    // finish jump
    if (isJumping && (newY < 0 + playerEntity->getHeight())) {
        eprintf("jump peak, current velY: %.2f; newPos would be: %.2f, %.2f\n", velocityY, newX, newY);
        velocityY = 0.0f;
    }
    if (isJumping && (newY > 240
            // || newY <   0 + playerEntity->getHeight()
            )) {
        eprintf("end jump, would be at: %.2f, %.2f\n", newX, newY);
        isJumping = false;
        velocityY = 0.0f;
        accelY = 0.0f;
    }

    // move, but keep in scene
    playerEntity->setPosition(
        std::clamp<float>(newX,
            0,
            400 - playerEntity->getWidth()),
        std::clamp<float>(newY,
            0 + playerEntity->getHeight(),
            240));

    // check collision with platforms
    Rectangle playerBbox = playerEntity->getRect();
    std::shared_ptr<Entity> platforms[] = {
        platform1Entity,
        platform2Entity,
        platform3Entity,
        platform4Entity,
    };

    for (auto platform : platforms) {
        Rectangle bbox = platform->getRect();

        /*printf("player: (%.02f, %.02f), (%.02f, %.02f)\n  platf1: (%.02f, %.02f), (%.02f, %.02f)\n",
			playerBbox.topLeft.x,
			playerBbox.topLeft.y,
			playerBbox.lowerRight.x,
			playerBbox.lowerRight.y,
			bbox.topLeft.x,
			bbox.topLeft.y,
			bbox.lowerRight.x,
			bbox.lowerRight.y
		);*/
        // playerEntity->getY()
        if (
            (
                playerEntity->getX() + playerEntity->getWidth() >= platform->getX() && playerEntity->getX() <= platform->getX() + platform->getWidth())
            && (playerEntity->getY() > platform->getY() - platform->getHeight())) {
            // if(playerBbox.lowerRight.y > bbox.topLeft.y) {
            printf("collide!\n");

            playerEntity->setPosition(
                playerEntity->getX(),
                platform->getY() - platform->getHeight());
            isJumping = false;
            velocityY = 0.0f;
            accelY = 0.0f;
        }
    }

    // if(durationElapsed < (1/30.0f)) {
    // frame limiter
    // return;
    // shouldRedraw = false;
    // eprintf("durationElapsed: + %.2f s\n", durationElapsed);
    // durationElapsed = 0;
    // sceneManager.switchFocusTo(nextSceneId);
    // } else {
    // shouldRedraw = true;
    // durationElapsed = 0;
    // }
}

void SceneGameplay::draw(RenderWindow& a_renderWindowUpper, RenderWindow& a_renderWindowLower)
{
    a_renderWindowUpper.beginDraw();
    a_renderWindowUpper.clear(C2D_Color32(255, 255, 255, 255));
    // a_renderWindowUpper.draw(playerEntity);
    a_renderWindowUpper.draw(myPlayer);
    a_renderWindowUpper.draw(platform1Entity);
    a_renderWindowUpper.draw(platform2Entity);
    a_renderWindowUpper.draw(platform3Entity);
    a_renderWindowUpper.draw(platform4Entity);

    a_renderWindowLower.beginDraw();
    a_renderWindowLower.clear(C2D_Color32(255, 255, 255, 255));
    a_renderWindowLower.draw(debugLabel);
}