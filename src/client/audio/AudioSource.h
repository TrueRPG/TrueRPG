#ifndef RPG_AUDIOSOURCE_H
#define RPG_AUDIOSOURCE_H

#include <mutex>
#include "IAudioClip.h"
#include "../../utils/Types.h"
#include "AudioState.h"

/**
 * Audio source class. Contains the necessary configuration to play audio.
 */
class AudioSource
{
    IAudioClip &m_audioClip;

    AudioState m_state{AudioState::Stop};

    float m_volume{1.f};
    float m_pan{0.f};
    bool m_loop{false};

    std::mutex m_mutex;

public:

    /**
     * Create an audio source.
     *
     * @param audioClip the audio clip
     */
    AudioSource(IAudioClip &audioClip);

    /**
     * Get the audio clip.
     *
     * @return the audio clip
     */
    IAudioClip &getAudioClip() const;

    /**
     * Get the current state.
     *
     * @return the current state
     */
    AudioState getState() const;

    /**
     * Play the audio clip.
     */
    void play();

    /**
     * Pause the audio clip.
     */
    void pause();

    /**
     * Stop the audio clip.
     */
    void stop();

    /**
     * Get the volume of the sound.
     * 0.f - no sound
     * 1.f - 100%
     *
     * @return the volume
     */
    float getVolume() const;

    /**
     * Set the volume of the sound.
     * 0.f - no sound
     * 1.f - 100%
     *
     * @param volume the volume
     */
    void setVolume(float volume);

    /**
     * Get the panning of the sound.
     * -1.f - left
     * 1.f - right
     *
     * @return the panning
     */
    float getPan() const;

    /**
     * Set the panning of the sound.
     * -1.f - left
     * 1.f - right
     *
     * @param pan the panning
     */
    void setPan(float pan);

    /**
     * Is the audio source is looped?
     *
     * @return true if it's looped and false otherwise
     */
    bool isLoop() const;

    /**
     * Make a loop .
     *
     * @param loop make a loop or not
     */
    void setLoop(bool loop);
};

#endif //RPG_AUDIOSOURCE_H
