#include "AudioDevice.h"

#include <iostream>
#include <algorithm>

AudioDevice::AudioDevice() : m_userData({m_sources, m_mutex})
{
    ma_device_config deviceConfig;
    deviceConfig = ma_device_config_init(ma_device_type_playback);
    deviceConfig.playback.format = FORMAT;
    deviceConfig.playback.channels = CHANNELS;
    deviceConfig.sampleRate = SAMPLE_RATE;
    deviceConfig.dataCallback = dataCallback;
    deviceConfig.pUserData = &m_userData;

    if (ma_device_init(nullptr, &deviceConfig, &m_device) != MA_SUCCESS)
    {
        std::cerr << "Failed to open playback device" << std::endl;
        ma_device_uninit(&m_device);
        return;
    }

    if (ma_device_start(&m_device) != MA_SUCCESS)
    {
        std::cerr << "Failed to start playback device" << std::endl;
        ma_device_uninit(&m_device);
        return;
    }
}

AudioDevice::~AudioDevice()
{
    clear();
    ma_device_uninit(&m_device);
}

void AudioDevice::add(AudioSource &source)
{
    auto result = m_sources.find(&source);
    if (result == m_sources.end())
    {
        m_mutex.lock();
        m_sources.insert({&source, {}});
        ma_decoder_config config = ma_decoder_config_init(FORMAT, CHANNELS, SAMPLE_RATE);
        source.getAudioClip().createDecoder(&m_sources[&source], &config);
        m_mutex.unlock();
    }
}

void AudioDevice::remove(AudioSource &source)
{
    auto result = m_sources.find(&source);
    if (result != m_sources.end())
    {
        m_mutex.lock();
        ma_decoder_uninit(&result->second);
        m_sources.erase(&source);
        m_mutex.unlock();
    }
}

void AudioDevice::clear()
{
    m_mutex.lock();
    for (auto &source : m_sources)
    {
        ma_decoder_uninit(&source.second);
    }
    m_sources.clear();
    m_mutex.unlock();
}

void AudioDevice::dataCallback(ma_device *pDevice, void *pOutput, const void *pInput, ma_uint32 frameCount)
{
    auto *userData = (UserData *) pDevice->pUserData;

    // Get all sources
    auto &sources = userData->sources;

    // Lock the mutex
    std::lock_guard<std::mutex> lockGuard(userData->mutex);

    if (sources.empty()) return;

    std::vector<AudioSource*> stoppedSources;

    // Mix all sources
    for (auto &item : sources)
    {
        if (item.first->getState() == AudioState::Play)
        {
            bool result = readAndMixSound(*item.first, item.second, (float *) pOutput, frameCount);
            if (!result)
            {
                item.first->stop();
            }
        }
        if (item.first->getState() == AudioState::Stop)
        {
            ma_decoder_seek_to_pcm_frame(&item.second, 0);
        }
    }
}

bool AudioDevice::readAndMixSound(const AudioSource &source, ma_decoder& decoder, float *pOutputF32, ma_uint32 frameCount)
{
    auto *temp = new float[frameCount * CHANNELS];

    ma_result result = ma_data_source_read_pcm_frames(&decoder, temp, frameCount, nullptr, source.isLoop());
    if (result != MA_SUCCESS)
    {
        return false;
    }

    // Mix the frames
    for (ma_uint32 sample = 0; sample < frameCount * CHANNELS; sample += CHANNELS)
    {
        // The volume of the left channel
        float left = 1 - std::clamp(source.getPan(), 0.f, 1.f);
        pOutputF32[sample] += source.getVolume() * left * temp[sample];
        std::clamp(pOutputF32[sample], -1.f, 1.f);

        // The volume of the right channel
        float right = 1 - std::abs(std::clamp(source.getPan(), -1.f, 0.f));
        pOutputF32[sample + 1] += source.getVolume() * right * temp[sample + 1];
        std::clamp(pOutputF32[sample + 1], -1.f, 1.f);
    }

    delete[] temp;

    return true;
}
