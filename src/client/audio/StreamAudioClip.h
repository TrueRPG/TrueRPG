#ifndef RPG_STREAMAUDIOCLIP_H
#define RPG_STREAMAUDIOCLIP_H

#include <string>
#include "IAudioClip.h"

 /**
  * This class doesn't store anything in memory, it plays the file directly from the disk.
  * It's better to use for long audio files.
  */
class StreamAudioClip : public IAudioClip
{
    std::string m_path;
public:

    /**
     * Create an audio clip.
     *
     * @param path the file path
     */
    StreamAudioClip(const std::string &path);

    virtual std::string getPath() const;

protected:
    virtual void createDecoder(ma_decoder* decoder, ma_decoder_config* config) const;
};

#endif //RPG_STREAMAUDIOCLIP_H
