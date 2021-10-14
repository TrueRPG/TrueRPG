#ifndef RPG_TEXTURE_H
#define RPG_TEXTURE_H

#include <string>
#include <glad/gl.h>
#include "IGLObject.h"

class Texture : public IGLObject
{
private:
    unsigned int m_id{};
    std::string m_path; // Путь до файла
    int m_width{}; // Ширина текстуры
    int m_height{}; // Высота текстуры

public:
    Texture();
    explicit Texture(unsigned int id, const std::string& path, int width, int height);

    void bind(unsigned int slot = 0) const;

    void unbind() const;

    unsigned int getId() const noexcept override;

    void destroy() override;

    std::string getPath() const;

    int getWidth() const;

    int getHeight() const;

    static Texture create(const std::string &path, unsigned int type = GL_TEXTURE_2D);
};


#endif //RPG_TEXTURE_H
