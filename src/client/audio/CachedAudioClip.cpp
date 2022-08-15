#include "../../pch.h"
#include "CachedAudioClip.h"

#include "AudioDevice.h"
#include <fstream>

CachedAudioClip::CachedAudioClip(const std::string &path)
        : m_path(path)
{
    std::ifstream input(path, std::ios::binary);
    m_data = std::vector((std::istreambuf_iterator<char>(input)), (std::istreambuf_iterator<char>()));
    input.close();
}

std::string CachedAudioClip::getPath() const
{
    return m_path;
}

void CachedAudioClip::createDecoder(ma_decoder* decoder, ma_decoder_config* config) const
{
    ma_decoder_init_memory(m_data.data(), m_data.size() * sizeof(char), config, decoder);
}

