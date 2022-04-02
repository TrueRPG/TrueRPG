#include "AudioSourceComponent.h"

AudioSourceComponent::AudioSourceComponent(IAudioClip &audioClip)
        : audioClip(&audioClip) {}

void AudioSourceComponent::play()
{
    state = AudioState::Play;
}

void AudioSourceComponent::pause()
{
    state = AudioState::Pause;
}

void AudioSourceComponent::stop()
{
    state = AudioState::Stop;
}
