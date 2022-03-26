#ifndef RPG_AUDIOSOURCECOMPONENT_H
#define RPG_AUDIOSOURCECOMPONENT_H

#include "../../client/audio/IAudioClip.h"
#include "../../utils/Types.h"
#include "../../client/audio/AudioState.h"

/**
 * Audio source component that is used to play an audio clip.
 */
struct AudioSourceComponent
{
    IAudioClip *audioClip;

    AudioState state{AudioState::Stop};

    float volume{1.f};
    float pan{0.f};
    bool loop{false};

    float maxDistance{2000.f};

public:

    /**
     * Create an audio source.
     *
     * @param audioClip the audio clip
     */
    AudioSourceComponent(IAudioClip &audioClip);

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
};

#endif //RPG_AUDIOSOURCECOMPONENT_H
