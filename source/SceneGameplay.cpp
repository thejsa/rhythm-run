#include "SceneGameplay.hpp"

SceneGameplay::SceneGameplay(SceneManager& a_sceneManager,
    AudioManager& a_audioManager, const char* a_trackName)
    : sceneManager(a_sceneManager)
    , audioManager(a_audioManager)
    , winScene(nullptr)
    , trackName(a_trackName)
    , winSceneId(0)
    , pauseSceneId(0)
    , durationEnd(1 / 30.0f)
    , durationElapsed(0.0f)
    , playerScore(0) {};

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

    bgSpriteSheet = C2D_SpriteSheetLoad("romfs:/gfx/LevelBg.t3x");
    if (!bgSpriteSheet) {
        eprintf("failed to load bg sprite sheet\n");
        // justSpin();
        svcBreak(USERBREAK_PANIC);
    }

    levelBgEntity = std::make_shared<Entity>(0, 240, // bottom left
        bgSpriteSheet, 0, // which sheet & image to load
        0.0f, 1.0f, // sprite's origin = bottom left
        1.0f, 1.0f, // scale
        0.0f //rotation
    );

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

    scoreLabel = std::make_shared<Label>("Score: 0", 384, 40,
        C2D_AtBaseline | C2D_AlignRight,
        1.0f, 1.0f, // font size
        C2D_Color32(0xff, 0xff, 0xff, 0xcc), // partially translucent
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

    // load beatmap bts file from romfs

    char btsPath[128];
    snprintf(btsPath, std::extent_v<decltype(btsPath)>, "romfs:/tracks/%s.bts", trackName);
    eprintf("opening bts %s\n", btsPath);

    std::ifstream btsHandle(btsPath);
    if (!btsHandle) {
        eprintf("Failed to open bts file at %s!!!\n", btsPath);
    } else {
        myBeatmap = BeatMap(btsHandle);
        eprintf("Opened BTS file at %s with %d beats\n", btsPath, myBeatmap.beats.size());
    }

    // load opus bgm file from romfs

    char bgmPath[128];
    snprintf(bgmPath, std::extent_v<decltype(bgmPath)>, "romfs:/tracks/%s.opus", trackName);
    eprintf("opening opus %s\n", bgmPath);

    opusFile = std::shared_ptr<OggOpusFile>(op_open_file(bgmPath, &error), op_free);

    if (error) {
        eprintf("Failed to open opus file! error: %d\n", error);
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
        audioManager.setLoop(true);
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

void SceneGameplay::setWinSceneId(unsigned int a_id)
{
    eprintf("%u\n", a_id);
    winSceneId = a_id;
}

void SceneGameplay::setWinScene(std::shared_ptr<SceneGameplayEnd> a_winScene)
{
    winScene = a_winScene;
}

void SceneGameplay::setPauseSceneId(unsigned int a_id)
{
    eprintf("%u\n", a_id);
    pauseSceneId = a_id;
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

    // pause
    if (kDown & KEY_START) {
        sceneManager.switchFocusTo(pauseSceneId);
    }
}

void SceneGameplay::update(float a_timeDelta)
{
    // move the platform1entity
    // get current position
    float p1epx = platform1Entity->getX();
    // move left at a fixed rate (per second)
    p1epx -= (192.0f * a_timeDelta);
    // wrap around
    if (p1epx < (0 - platform1Entity->getWidth())) {
        p1epx += 400;
    }

    platform1Entity->setPosition(
        p1epx,
        216);

    // get audio length
    static ogg_int64_t totalPcm = OP_EINVAL;
    if (totalPcm == OP_EINVAL)
        totalPcm = op_pcm_total(opusFile.get(), -1);

    static char debugLabelBuf[256];
    static char scoreLabelBuf[256];
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

    // get time in audio
    // sample rate = 48kHz]
    ogg_int64_t tellPcm = op_pcm_tell(opusFile.get());

    // variables for the next beat to be sent on screen
    static long nextBeat = -1;
    static long showNextBeatAt = -1;

    // pop beat off the front of the beatmap queue if we need another
    if (nextBeat == -1) {
        nextBeat = myBeatmap.popBeat();
        eprintf("next beat: %ld\n", nextBeat);

        if (nextBeat == -1) {
            // eprintf("no more beats, endgame??");
            showNextBeatAt = -1;
        } else {
            // should have 3 beats ahead; bpm/60 = bps; s = bps/b
            showNextBeatAt = nextBeat - (3 / 60.0f * myBeatmap.bpm);
        }
    }

    // 48kHz = 48 cycles per ms
    if (showNextBeatAt > -1 && (tellPcm / 48.0f) >= showNextBeatAt) {
        auto theBeatEntity = std::make_unique<Entity>(
            (400 - 32), 128, // top right ish and it'll move left
            spriteSheet, 4, // which sheet & image to load
            // 0.5f, 0.5f, // sprite's origin
            0.0f, 1.0f, // sprite's origin -- bottom left
            1.0f, 1.0f, // scale
            0.0f //rotation
        );
        auto theBeat = std::make_shared<Beat>(
            nextBeat,
            theBeatEntity);
        // velocity equals distance over time
        // (-400 + 32) / (nextBeat - showNextBeatAt)
        theBeat->speed.x = (32.0f - 400.0f) / (1 / 10.0f * (nextBeat - showNextBeatAt));
        eprintf("new x speed: %.05f\n", theBeat->speed.x);

        beatQueue.push_back(theBeat);

        nextBeat = -1;
        showNextBeatAt = -1;
    }

    // remove expired beats and run collision detection
    for (auto& theBeat : beatQueue) {
        theBeat->update(a_timeDelta);
        // remove expired beats
        if (theBeat->position.x < -32.0f) {
            beatQueue.pop_front();
        } else {
            // collision detection (if player hit the beat, increment score)
            if (myPlayer->myEntity->getAABB().doesOverlap(beatQueue.front()->myEntity->getAABB())) {
                playerScore += 100;
                // vanish the beat by moving off-screen (will be deleted next frame)
                beatQueue.front()->position.x = -64.0f;
            }
        }
    }

    // check if player has hit the front beat (if any)
    // if (!beatQueue.empty() && myPlayer->myEntity->getAABB().doesOverlap(beatQueue.front()->myEntity->getAABB())) {
    //     // nice!
    //     playerScore += 100;
    // }

    // update score label on top screen
    int scoreLabelSize = sprintf(scoreLabelBuf, "Score: %ld", playerScore);
    scoreLabel->setText(scoreLabelBuf, scoreLabelSize);

    if (tellPcm == totalPcm) {
        // end of track!
        printf("end game! score: %ld\n", playerScore);
        winScene.get()->setScore(playerScore);
        // winScene->setScore(playerScore);
        sceneManager.switchFocusTo(winSceneId);
    }

    int debugLabelSize = sprintf(debugLabelBuf, "SCORE: %ld\n\nv: (%.02f, %.02f)\na: (%.02f, %.02f)\np: (%.02f, %.02f)\nstate: %s\nisOnGround?: %d\naudio: %.02f ss / %.02f s\ne: %.02f, %.02f\nnextBeat: %ld / showNextBeatAt: %ld\nqueue length: %d\n",
        playerScore,
        myPlayer->speed.x,
        myPlayer->speed.y,
        myPlayer->accel.x,
        myPlayer->accel.y,
        myPlayer->position.x,
        myPlayer->position.y,
        currentState,
        myPlayer->isOnGround,
        tellPcm / 48000.0f,
        totalPcm / 48000.0f,
        platform1Entity->getX(), platform1Entity->getY(),
        nextBeat, showNextBeatAt, beatQueue.size());
    debugLabel->setText(debugLabelBuf, debugLabelSize);

    // change scene if splash screen should end
    // if(durationElapsed >= durationEnd)
    // 	sceneManager.switchFocusTo(winSceneId);
}

void SceneGameplay::draw(RenderWindow& a_renderWindowUpper, RenderWindow& a_renderWindowLower)
{
    // begin drawing and clear the screen
    a_renderWindowUpper.beginDraw();
    a_renderWindowUpper.clear(C2D_Color32(255, 255, 255, 255));

    // draw the player and background sprites
    a_renderWindowUpper.draw(levelBgEntity);
    a_renderWindowUpper.draw(myPlayer);
    // a_renderWindowUpper.draw(platform1Entity);

    // draw all the beats in the beat queue
    for (auto& theBeat : beatQueue) {
        a_renderWindowUpper.draw(theBeat);
    }

    // draw the score label :)
    a_renderWindowUpper.draw(scoreLabel);

    // a_renderWindowUpper.draw(platform2Entity);
    // a_renderWindowUpper.draw(platform3Entity);
    // a_renderWindowUpper.draw(platform4Entity);

    a_renderWindowLower.beginDraw();
    a_renderWindowLower.clear(C2D_Color32(255, 255, 255, 255));
    a_renderWindowLower.draw(debugLabel);
}