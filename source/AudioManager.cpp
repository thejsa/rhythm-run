#include "AudioManager.hpp"

#include <opusfile.h>

/// Constructor
AudioManager::AudioManager()
:shouldStop(true), isSkipping(false), files(0),
currentFile(nullptr), fileCounter(0) {
    eprintf("init\n");
    ndspInit();

    auto const bufferSize = WAVEBUF_SIZE * std::extent_v<decltype(this->waveBufs)>;

    this->audioBuffer = static_cast<int16_t*>(linearAlloc(bufferSize));
    if(!this->audioBuffer) {
        eprintf("Failed to allocate audio buffer\n");
    }
}

AudioManager::~AudioManager() {
    eprintf("cleanup\n");
    this->stop();
    this->files.clear();
    eprintf("all done!\n");
}

void AudioManager::stop() {
    eprintf("stop called!\n");
    this->shouldStop = true;

    // signal playback thread
    LightEvent_Signal(&this->audioEvent);

    // join playback thread, await its completion and then free it
    threadJoin(this->threadId, U64_MAX);

    // get exit code
    int exitCode = threadGetExitCode(this->threadId);
    if(exitCode != 0) {
        // error!
        eprintf("ERROR: thread returned exit code %d\n", exitCode);
    }

    threadFree(this->threadId);

    ndspSetCallback(NULL, nullptr);
    ndspChnReset(BGM_CHANNEL);
    this->threadId = NULL;
}

void AudioManager::play() {
    // Create audio thread
    s32 priority = 0x30;
	svcGetThreadPriority (&priority, CUR_THREAD_HANDLE);
	priority = std::clamp<s32> (priority - 1, 0x18, 0x3F);

	this->threadId = threadCreate (proxyAudioThread, this, THREAD_STACK_SIZE, priority, THREAD_AFFINITY, false);
    eprintf("created thread with id %x\n", this->threadId);

    // eprintf("pl\n");
    // // this->shouldPause = false;
    // this->isSkipping = false;
    // this->shouldStop = false;
    // LightEvent_Signal(&this->playEvent);
}

bool AudioManager::fillBuffer(OggOpusFile *const p_opusFile, ndspWaveBuf &p_waveBuf) {
    // eprintf("!\n");
    // #ifdef DEBUG
    // TickCounter timer;
    // osTickCounterStart(&timer);
    // #endif

    int totalSamples = 0;
    while (totalSamples < SAMPLES_PER_BUF) {
        auto const buffer = p_waveBuf.data_pcm16 + totalSamples * CHANNELS_PER_SAMPLE;
        auto const bufferSize = (SAMPLES_PER_BUF - totalSamples) * CHANNELS_PER_SAMPLE;

        // decode samples
        auto const samples = op_read_stereo(p_opusFile, buffer, bufferSize);
        // eprintf("decoded %d samples\n", samples);
        
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

    // #ifdef DEBUG
    // osTickCounterUpdate(&timer);
    // eprintf("fillBuffer %lf ms in %lf ms\n", totalSamples * 1000.0 / SAMPLE_RATE, osTickCounterRead(&timer));
    // #endif
    
    return true;
};

void AudioManager::audioThread() {
    // auto const opusFile = static_cast<OggOpusFile*>(p_arg);
    eprintf("Hello from audio thread!\n");
    if(!this->currentFile) {
        eprintf("no file!\n");

        // exit
        this->shouldStop = true;
        threadExit(1); // error state
    }

    // Initialise sync primitive
    LightEvent_Init(&audioEvent, RESET_ONESHOT);

    // Init stuff for playback
    this->initPlayback();

    eprintf("let's go!\n");
    this->shouldStop = false;

    while(!this->shouldStop && !this->isSkipping) {

        // yield
        svcSleepThread(0);


        // eprintf("!\n");
        // if(this->shouldPause) {
        //     eprintf("paused");
        //     LightEvent_Wait(&this->playEvent);
        //     eprintf("play!");
        //     continue;
        // }

        for(auto &waveBuf : this->waveBufs) {
            // yield
            svcSleepThread(0);

            if(waveBuf.status != NDSP_WBUF_DONE) continue;

            // fill buffer, or break if end of file
            if(!this->fillBuffer(this->currentFile.get(), waveBuf)) {
                eprintf("reached end of audio playback\n");
                this->shouldStop = true;
                break;
            }

            // add the wavebuf and flush DSP cache
            ndspChnWaveBufAdd(BGM_CHANNEL, &waveBuf);
            DSP_FlushDataCache(waveBuf.data_pcm16, waveBuf.nsamples * CHANNELS_PER_SAMPLE * sizeof(std::int16_t));
        }

        // If playback is paused, spin
        // if(ndspChnIsPaused(BGM_CHANNEL)) {
        //     continue;
        // }

        // wait until we're needed!
        LightEvent_Wait(&this->audioEvent);

        // yield
        svcSleepThread(0);
    }

    // by this point, we've reached end of file, or have been told to stop
    // reset audio channel
    ndspChnReset(BGM_CHANNEL);

    // exit
    this->shouldStop = true;
    threadExit(0);
}

void AudioManager::initPlayback() {
    eprintf("hi!\n");
    // Initialise NDSP
    ndspChnReset(BGM_CHANNEL);
    ndspSetOutputMode(NDSP_OUTPUT_STEREO);
    ndspChnSetInterp(BGM_CHANNEL, NDSP_INTERP_POLYPHASE);
    ndspChnSetRate(BGM_CHANNEL, SAMPLE_RATE);
    ndspChnSetFormat(BGM_CHANNEL, NDSP_FORMAT_STEREO_PCM16);
    ndspSetCallback(proxyAudioCallback, this);
    eprintf("completed ndsp init\n");

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

int AudioManager::switchFileTo(unsigned int p_fileId) {
    // Tear down any running playback thread
    if(this->threadId != NULL)
        this->stop();
    
    // Seek any active audio file to the start
    if(this->currentFile)
        op_raw_seek(this->currentFile.get(), 0);

    // Find the id-file pair for p_fileId
	eprintf("%u\n", p_fileId);
    auto filePair = this->files.find(p_fileId);

    // Validate that the file was found
    if(filePair == this->files.end()) {
        eprintf("file id %d not found!!\n", p_fileId);
        // svcBreak(USERBREAK_PANIC);
        return 1;
    }
    
    // Set current file to new file
    this->currentFile = filePair->second;

    // this->shouldPause = false;
    // LightEvent_Signal(&this->playEvent);
    // if(!wasPaused) this->play();

    // TODO: reset audio position -- done?

    // Call onFocus
    // this->currentFile->onFocus();
    return 0;
}