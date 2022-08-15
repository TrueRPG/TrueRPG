#ifndef RPG_CACHEDAUDIOCLIP_H
#define RPG_CACHEDAUDIOCLIP_H

#include <string>
#include <vector>
#include "IAudioClip.h"

/**
 * This class caches an audio clip in memory.
 * It's better to use for short audio files.
 */
class CachedAudioClip : public IAudioClip
{
    std::string m_path;
    std::vector<char> m_data;

public:

    /**
     * Create an audio clip.
     *
     * @param path the file path
     */
    CachedAudioClip(const std::string &path);

    virtual std::string getPath() const;

protected:
    virtual void createDecoder(ma_decoder* decoder, ma_decoder_config* config) const;
};

#endif //RPG_CACHEDAUDIOCLIP_H
