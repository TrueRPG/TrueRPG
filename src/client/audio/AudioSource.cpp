#include "../../pch.h"
#include "AudioSource.h"

AudioSource::AudioSource(IAudioClip &audioClip)
    : m_audioClip(audioClip) {}

IAudioClip& AudioSource::getAudioClip() const
{
    return m_audioClip;
}

AudioState AudioSource::getState() const
{
    return m_state;
}

void AudioSource::play()
{
    m_mutex.lock();
    m_state = AudioState::Play;
    m_mutex.unlock();
}

void AudioSource::pause()
{
    m_mutex.lock();
    m_state = AudioState::Pause;
    m_mutex.unlock();
}

void AudioSource::stop()
{
    m_mutex.lock();
    m_state = AudioState::Stop;
    m_mutex.unlock();
}

float AudioSource::getVolume() const
{
    return m_volume;
}

void AudioSource::setVolume(float volume)
{
    m_mutex.lock();
    m_volume = volume;
    m_mutex.unlock();
}

float AudioSource::getPan() const
{
    return m_pan;
}

void AudioSource::setPan(float pan)
{
    m_mutex.lock();
    m_pan = pan;
    m_mutex.unlock();
}

bool AudioSource::isLoop() const
{
    return m_loop;
}

void AudioSource::setLoop(bool loop)
{
    m_mutex.lock();
    m_loop = loop;
    m_mutex.unlock();
}
