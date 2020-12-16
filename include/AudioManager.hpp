#pragma once

#include <3ds.h>

#include <algorithm>
// #include <atomic>

#include <cstdlib>
#include <cstdint>
#include <cstring>

#include <memory>
#include <type_traits>
#include <unordered_map>

#include <opusfile.h>

#include "debug.hpp"

/// Finite state machine to handle background music management
class AudioManager
{
public:
    AudioManager();

    // Passthrough function to execute audioThread instance method from threadCreate
    static void proxyAudioThread(void* ctx) {
        static_cast<AudioManager*>(ctx)->audioThread();
    };
    // Passthrough function to execute audioCallback instance method from NDSP
    static void proxyAudioCallback(void* ctx) {
        static_cast<AudioManager*>(ctx)->audioCallback();
    };

    // Add file to the state machine, returning its ID
    unsigned int addFile(std::shared_ptr<OggOpusFile> p_file);
    // Transition focus to file
    void switchFileTo(unsigned int p_fileId);
    // Remove file from the state machine
    void removeFile(unsigned int p_fileId);

    void cleanup() {
        this->shouldQuit = true;
        threadJoin (this->threadId, UINT64_MAX);
        threadFree (this->threadId);

        ndspChnWaveBufClear(0);
        linearFree(audioBuffer);

        ndspExit();
    }

    void play() {
        eprintf("pl\n");
        this->shouldPause = false;
        LightEvent_Signal(&this->playEvent);
    }
    void pause() {
        eprintf("pa\n");
        this->shouldPause = true;
    }
private:
    LightEvent audioEvent;
    LightEvent playEvent;

    // default to paused
    // todo: switch to std::atomic
    bool shouldPause = true;
    bool shouldQuit  = false;

    ndspWaveBuf waveBufs[3];
    int16_t *audioBuffer = nullptr;
    
    Thread threadId;

    bool fillBuffer(OggOpusFile *const p_opusFile, ndspWaveBuf &p_waveBuf);

    void audioThread();
    void audioCallback() {
        if(shouldQuit) return;
        printf("c\n");
        LightEvent_Signal(&this->audioEvent);
    };
    
    /// Stores all files
    std::unordered_map<unsigned int, std::shared_ptr<OggOpusFile>> files;
    /// Stores a reference to the current file
    std::shared_ptr<OggOpusFile> currentFile;
    /// Stores the current file ID, incremented whenever a file is added
    unsigned int fileCounter;

    // Defines
    static constexpr auto SAMPLE_RATE = 48000;
    static constexpr auto BUFFER_MS   = 120;

    static constexpr auto SAMPLES_PER_BUF = SAMPLE_RATE * BUFFER_MS / 1000;
    static_assert(SAMPLES_PER_BUF == 5760);
    
    static constexpr auto CHANNELS_PER_SAMPLE = 2;

    static constexpr auto WAVEBUF_SIZE = SAMPLES_PER_BUF * CHANNELS_PER_SAMPLE * sizeof(std::int16_t);
    
    // threading
    static constexpr auto THREAD_AFFINITY = -1;
    static constexpr auto THREAD_STACK_SIZE = 32 * 1024;
};