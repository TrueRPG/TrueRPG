#ifndef RPG_IAUDIOCLIP_H
#define RPG_IAUDIOCLIP_H

#include <miniaudio.h>
#include <string>

/**
 * Audio clip interface.
 */
class IAudioClip
{
public:
    virtual ~IAudioClip() = default;

    /**
     * Get the audio file path.
     *
     * @return the file path
     */
    virtual std::string getPath() const = 0;

protected:

    /**
     * Create a decoder for the audio clip.
     *
     * @param decoder the decoder
     * @param config the config
     */
    virtual void createDecoder(ma_decoder* decoder, ma_decoder_config* config) const = 0;

    friend class AudioDevice;
};

#endif //RPG_IAUDIOCLIP_H
