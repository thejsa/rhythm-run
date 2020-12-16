#include "AudioManager.hpp"

#include <opusfile.h>

/// Constructor
AudioManager::AudioManager()
:files(0), currentFile(nullptr), fileCounter(0) {
    eprintf("init\n");
    ndspInit();
    
    ndspChnReset(0);
    ndspSetOutputMode(NDSP_OUTPUT_STEREO);
    ndspChnSetInterp(0, NDSP_INTERP_POLYPHASE);
    ndspChnSetRate(0, SAMPLE_RATE);
    ndspChnSetFormat(0, NDSP_FORMAT_STEREO_PCM16);

    auto const bufferSize = WAVEBUF_SIZE * std::extent_v<decltype(this->waveBufs)>;

    this->audioBuffer = static_cast<int16_t*>(linearAlloc(bufferSize));
    if(!this->audioBuffer) {
        eprintf("Failed to allocate audio buffer\n");
    }

    // Get a pointer to the audio buffer that we can advance
    auto buffer = this->audioBuffer;
    eprintf("allocated buffer %x\n", buffer);

    // Initialise wavebufs
    std::memset(&this->waveBufs, 0, sizeof(this->waveBufs));

    for(auto &waveBuf : this->waveBufs) {
        waveBuf.data_vaddr = buffer;
        waveBuf.status     = NDSP_WBUF_DONE;

        buffer += WAVEBUF_SIZE / sizeof(buffer[0]);
    }

    ndspSetCallback(proxyAudioCallback, this);
    eprintf("set ndsp callback\n", buffer);
    
    s32 priority = 0x30;
	svcGetThreadPriority (&priority, CUR_THREAD_HANDLE);
	priority = std::clamp<s32> (priority - 1, 0x18, 0x3F);

	this->threadId = threadCreate (proxyAudioThread, this, THREAD_STACK_SIZE, priority, THREAD_AFFINITY, false);
    eprintf("created thread with id %x\n", buffer, this->threadId);
}

bool AudioManager::fillBuffer(OggOpusFile *const p_opusFile, ndspWaveBuf &p_waveBuf) {
    eprintf("!\n");
    // #ifdef DEBUG
    TickCounter timer;
    osTickCounterStart(&timer);
    // #endif

    int totalSamples = 0;
    while (totalSamples < SAMPLES_PER_BUF) {
        auto const buffer = p_waveBuf.data_pcm16 + totalSamples * CHANNELS_PER_SAMPLE;
        auto const bufferSize = (SAMPLES_PER_BUF - totalSamples) * CHANNELS_PER_SAMPLE;

        // decode samples
        auto const samples = op_read_stereo(p_opusFile, buffer, bufferSize);
        eprintf("decoded %d samples\n", samples);
        
        if(samples <= 0)
            break;
        
        totalSamples += samples;
    }

    if(totalSamples == 0)
    {
        eprintf("Playback complete\n");
        return false;
        // TODO: Looping audio
    }

    p_waveBuf.nsamples = totalSamples;
    ndspChnWaveBufAdd(0, &p_waveBuf);
    DSP_FlushDataCache(p_waveBuf.data_pcm16, totalSamples * CHANNELS_PER_SAMPLE * sizeof(std::int16_t));

    // #ifdef DEBUG
    osTickCounterUpdate(&timer);
    eprintf("fillBuffer %lf ms in %lf ms\n", totalSamples * 1000.0 / SAMPLE_RATE, osTickCounterRead(&timer));
    // #endif
    
    return true;
};

void AudioManager::audioThread() {
    // auto const opusFile = static_cast<OggOpusFile*>(p_arg);
    eprintf("hello\n");
    LightEvent_Wait(&this->playEvent);
    eprintf("let's go!\n");
    while(!this->shouldQuit) {
        eprintf("!\n");
        if(this->shouldPause) {
            eprintf("paused");
            LightEvent_Wait(&this->playEvent);
            eprintf("play!");
            continue;
        }

        for(auto &waveBuf : this->waveBufs) {
            if(waveBuf.status != NDSP_WBUF_DONE) continue;

            if(!this->fillBuffer(this->currentFile.get(), waveBuf)) return;
        }
        LightEvent_Wait(&this->audioEvent);
    }
}

unsigned int AudioManager::addFile(std::shared_ptr<OggOpusFile> p_file) {
	eprintf("id:%u p:%x\n", this->fileCounter, p_file);
    /// Add an id-file pair to the files map
    // auto insertedFile = this->files.insert(std::make_pair(this->fileCounter, p_file));
    this->files.insert(std::make_pair(this->fileCounter, p_file));

    /// Call onCreate of the file we just inserted
    // insertedFile.first->second->onCreate();

    /// Return, and then increment, the file counter
    return this->fileCounter++;
};

void AudioManager::removeFile(unsigned int p_fileId) {
    // Find the id-file pair for p_fileId
	eprintf("id:%u\n", p_fileId);
    auto filePair = this->files.find(p_fileId);

    if(filePair != this->files.end()) {
        // If file being removed is the current file, set it to nullptr
        // to make sure we don't try to continue updating a nonexistant file
        if(this->currentFile == filePair->second) {
            this->currentFile = nullptr;
        }

        // Call file's onDestroy method so it can deinit things, etc
        // filePair->second->onDestroy();

        // Erase the file from the files map
        this->files.erase(filePair);
    }
}

void AudioManager::switchFileTo(unsigned int p_fileId) {
    // Find the id-file pair for p_fileId
	eprintf("%u\n", p_fileId);
    auto filePair = this->files.find(p_fileId);

    bool wasPaused = this->shouldPause;

    if(filePair != this->files.end()) {
        // Call current file's onBlur if necessary
        // eprintf("ptr:%u\n", filePair->second);
        if(this->currentFile) {
            // this->shouldPause = true;
            if(!wasPaused) this->pause();

            // Reset it to the start
            op_raw_seek(this->currentFile.get(), 0);
            // eprintf("blurring old file\n");
            // this->currentFile->onBlur();
        }
    }
    
    // Set current file to new file
    this->currentFile = filePair->second;

    // this->shouldPause = false;
    // LightEvent_Signal(&this->playEvent);
    if(!wasPaused) this->play();

    // TODO: reset audio position -- done?

    // Call onFocus
    // this->currentFile->onFocus();
}