#include "Texture.h"

#include <iostream>
#include <glad/gl.h>
#include <stb_image.h>

Texture::Texture() : m_id(0), m_path(""), m_width(0), m_height(0) { }

Texture::Texture(unsigned int id, const std::string& path, int width, int height) 
        : m_id(id), 
        m_path(path), 
        m_width(width), 
        m_height(height) { }

void Texture::bind(unsigned int slot) const
{
    glBindTextureUnit(slot, m_id);
}

void Texture::unbind() const
{
    glBindTextureUnit(0, 0);
}

unsigned int Texture::getId() const noexcept
{
    return m_id;
}

void Texture::destroy()
{
    glDeleteTextures(1, &m_id);
    m_id = 0;
}

std::string Texture::getPath() const
{
    return m_path;
}

int Texture::getWidth() const
{
    return m_width;
}

int Texture::getHeight() const
{
    return m_height;
}

// Нам может пригодиться тип текстур GL_TEXTURE_RECTANGLE и GL_TEXTURE_2D
Texture Texture::create(const std::string& path, unsigned int type)
{
    unsigned int texture;
    int channels;
    int width;
    int height;
    unsigned char* data;

    glCreateTextures(type, 1, &texture);

    // Установка параметров наложения текстуры
    glTextureParameteri(texture, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTextureParameteri(texture, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTextureParameteri(texture, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTextureParameteri(texture, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    stbi_set_flip_vertically_on_load(1);

    data = stbi_load(path.c_str(), &width, &height, &channels, 0);
    if (!data)
    {
         std::cout << "Failed to load texture" << std::endl;
         return Texture(0, "", 0, 0);
    }

    glTextureStorage2D(texture, 1, GL_RGBA8, width, height);
    glTextureSubImage2D(texture, 0, 0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glGenerateTextureMipmap(texture);
    
    stbi_image_free(data);

    return Texture(texture, path, width, height);
}
