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
    ~AudioManager();

    // Passthrough function to execute audioThread instance method from threadCreate
    static inline void proxyAudioThread(void* a_ctx) {
        static_cast<AudioManager*>(a_ctx)->audioThread();
    };
    // Passthrough function to execute audioCallback instance method from NDSP
    static inline void proxyAudioCallback(void* a_ctx) {
        static_cast<AudioManager*>(a_ctx)->audioCallback();
    };

    // Add file to the state machine, returning its ID
    unsigned int addFile(std::shared_ptr<OggOpusFile> a_file);
    // Transition focus to file
    // Retval: 0 for success, otherwise error
    int switchFileTo(unsigned int a_fileId);
    // Remove file from the state machine
    void removeFile(unsigned int a_fileId);

    void cleanup() {
        this->shouldStop = true;
        threadJoin (this->threadId, UINT64_MAX);
        threadFree (this->threadId);

        ndspChnWaveBufClear(0);
        linearFree(audioBuffer);

        ndspExit();
    }

    /// Stop playback and free the playback thread.
    void stop();
    /// Is playback 'paused' (whether stopped or not playing)?
    inline bool isStopped() {
        return this->shouldStop;
    };
    inline bool isPaused() {
        return this->isStopped() ? true : ndspChnIsPaused(BGM_CHANNEL);
    };

    void play();
    // void pause() {
    //     this->shouldStop = true;
    // }
    inline void pause() {
        eprintf("pa\n");
        // this->shouldPause = true;
        ndspChnSetPaused(BGM_CHANNEL, true);
    };
    inline void unpause() {
        eprintf("upa\n");
        // this->shouldPause = true;
        ndspChnSetPaused(BGM_CHANNEL, false);
        LightEvent_Signal(&this->audioEvent);
    };
    inline void togglePause() {
        eprintf("tpa\n");
        // this->shouldPause = true;
        if(!this->shouldStop)
            ndspChnSetPaused(BGM_CHANNEL, !this->isPaused());
        LightEvent_Signal(&this->audioEvent);
    };

    /// LightEvent for signalling the audio thread
    LightEvent audioEvent;
    /// Should playback be stopped?
    bool shouldStop;
    /// Is playback currently switching audio file?
    bool isSkipping;

    inline void audioCallback() {
        // Do not signal the audio thread if we want to interrupt its work
        // (e.g, when playback is stopped or if we're changing audio data)
        if(this->shouldStop || this->isSkipping)
            return;
        
        // Signal the audio thread to do more work
        LightEvent_Signal(&audioEvent);
        // svcSleepThread(0);
    }
private:
    void initPlayback();
    void doPlayback();

    LightEvent playEvent;

    // default to paused
    // todo: switch to std::atomic
    // bool shouldPause = true;
    // bool shouldQuit  = false;

    ndspWaveBuf waveBufs[3];
    int16_t *audioBuffer = nullptr;
    
    Thread threadId;

    bool fillBuffer(OggOpusFile *const a_opusFile, ndspWaveBuf &a_waveBuf);

    void audioThread();
    
    /// Stores all files
    std::unordered_map<unsigned int, std::shared_ptr<OggOpusFile>> files;
    /// Stores a reference to the current file
    std::shared_ptr<OggOpusFile> currentFile;
    /// Stores the current file ID, incremented whenever a file is added
    unsigned int fileCounter;

    // Defines
    static constexpr auto BGM_CHANNEL = 0;
    
    static constexpr auto SAMPLE_RATE = 48000;
    static constexpr auto BUFFER_MS   = 120;

    static constexpr auto SAMPLES_PER_BUF = SAMPLE_RATE * BUFFER_MS / 1000;
    static_assert(SAMPLES_PER_BUF == 5760);
    
    static constexpr auto CHANNELS_PER_SAMPLE = 2;

    static constexpr auto WAVEBUF_SIZE = SAMPLES_PER_BUF * CHANNELS_PER_SAMPLE * sizeof(std::int16_t);
    
    // threading
    static constexpr auto THREAD_AFFINITY = -2;
    static constexpr auto THREAD_STACK_SIZE = 32 * 1024;
};