#include "../../pch.h"
#include "Texture.h"

#include <iostream>
#include "Graphics.h"
#include <stb_image.h>

#include "Bitmap.h"

Texture::Texture() : m_id(0), m_path(""), m_width(0), m_height(0) { }

Texture::Texture(unsigned int id, const std::string& path, int width, int height) 
        : m_id(id), 
        m_path(path), 
        m_width(width), 
        m_height(height) { }

void Texture::bind(unsigned int slot) const
{
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, m_id);
}

void Texture::unbind() const
{
    glBindTexture(GL_TEXTURE_2D, 0);
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

// GL_TEXTURE_RECTANGLE and GL_TEXTURE_2D might be useful for us
Texture Texture::create(const std::string& path, unsigned int type)
{
    unsigned int texture;
    int channels;
    int width;
    int height;
    unsigned char* data;

    glGenTextures(1, &texture);
    glBindTexture(type, texture);

    // Set up the texture params
    glTexParameteri(type, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(type, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(type, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(type, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    stbi_set_flip_vertically_on_load(1);

    data = stbi_load(path.c_str(), &width, &height, &channels, 0);
    if (!data)
    {
         std::cout << "Failed to load texture" << std::endl;
         return Texture(0, "", 0, 0);
    }

    glTexImage2D(type, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(type);
    
    stbi_image_free(data);

    return Texture(texture, path, width, height);
}

Texture Texture::createEmpty()
{
    static unsigned int texture = -1;

    if (texture == -1)
    {
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);

        // Set up the texture params
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        unsigned char pixel[]{255, 255, 255, 255};
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixel);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    return Texture(texture, "no_path", 1, 1);
}

Texture Texture::create(const Bitmap &bitmap, unsigned int type)
{
    unsigned int texture;

    glGenTextures(1, &texture);
    glBindTexture(type, texture);

    // Set up the texture params
    glTexParameteri(type, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(type, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(type, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(type, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTexImage2D(type, 0, GL_RGBA8, bitmap.getWidth(), bitmap.getHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, bitmap.getRawPixels().data());
    glGenerateMipmap(type);

    return Texture(texture, "no_path", bitmap.getWidth(), bitmap.getHeight());
}
