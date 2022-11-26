#ifndef RPG_TEXTURE_H
#define RPG_TEXTURE_H

#include <string>
#include "../Graphics.h"
#include "../ITexture.h"

class Bitmap;

class Texture : public ITexture
{
private:
    unsigned int m_id{};
    std::string m_path; // The file path
    int m_width{}; // The width of the texture
    int m_height{}; // The height of the texture

public:
    Texture();
    explicit Texture(unsigned int id, const std::string& path, int width, int height);

    void bind(unsigned int slot = 0) const override;

    void unbind() const override;

    unsigned int getId() const noexcept override;

    void destroy() override;

    std::string getPath() const override;

    int getWidth() const override;

    int getHeight() const override;

    static Texture create(const std::string &path, unsigned int type = GL_TEXTURE_2D);

    static Texture createEmpty();

    static Texture create(const Bitmap &bitmap, unsigned int type = GL_TEXTURE_2D);
};


#endif //RPG_TEXTURE_H
