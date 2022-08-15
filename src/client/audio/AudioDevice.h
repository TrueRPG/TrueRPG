#ifndef RPG_AUDIODEVICE_H
#define RPG_AUDIODEVICE_H

#include "AudioSource.h"
#include "AudioState.h"

#include <miniaudio.h>
#include <mutex>
#include <unordered_map>

#define FORMAT ma_format_f32
#define CHANNELS 2
#define SAMPLE_RATE 48000

// The structure for passing data into the decoder callback function
struct UserData
{
    std::unordered_map<AudioSource *, ma_decoder> &sources;
    std::mutex &mutex;
};

 /**
  * Audio device class.
  * It can play sounds and mix them with each other.
  * Ideally, there should be only one audio device for the entire game.
  */
class AudioDevice
{
private:
    ma_device m_device{};

    // This audio source map is a shared resource,
    // because sounds must be played in a separate thread.
    // So be careful with this.
    std::unordered_map<AudioSource *, ma_decoder> m_sources;

    std::mutex m_mutex;

    // This data we want to pass into the decoder callback function
    UserData m_userData;

public:

    /**
     * Create an audio device.
     */
    AudioDevice();

    ~AudioDevice();

    /**
     * Add an audio source.
     *
     * @param source the audio source
     */
    void add(AudioSource &source);

    /**
     * Remove an audio source.
     *
     * @param source the audio source
     */
    void remove(AudioSource &source);

    /**
     * Clear the device.
     */
    void clear();

private:
    static void dataCallback(ma_device *pDevice, void *pOutput, const void *pInput, ma_uint32 frameCount);

    static bool readAndMixSound(const AudioSource &source, ma_decoder &decoder, float *pOutputF32, ma_uint32 frameCount);
};

#endif //RPG_AUDIODEVICE_H
