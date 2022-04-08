#include "Sprite.h"

Sprite::Sprite()
    : m_textureRect(0, 0, 1, 1),
      m_texture(Texture::createEmpty())
{
}

Sprite::Sprite(Texture &texture)
    : m_textureRect(0, 0, texture.getWidth(), texture.getHeight()),
      m_texture(texture)
{
}

glm::vec2 Sprite::getPosition() const
{
    return m_position;
}

void Sprite::setPosition(glm::vec2 position)
{
    m_position = position;
}

glm::vec2 Sprite::getOrigin() const
{
    return m_origin;
}

void Sprite::setOrigin(glm::vec2 origin)
{
    m_origin = origin;
}

glm::vec2 Sprite::getScale() const
{
    return m_scale;
}

void Sprite::setScale(glm::vec2 scale)
{
    m_scale = scale;
}

glm::vec4 Sprite::getColor() const
{
    return m_color;
}

void Sprite::setColor(glm::vec4 color)
{
    m_color = color;
}

Texture Sprite::getTexture() const
{
    return m_texture;
}

IntRect Sprite::getTextureRect() const
{
    return m_textureRect;
}

void Sprite::setTextureRect(const IntRect &rect)
{
    m_textureRect = rect;
}

FloatRect Sprite::getLocalBounds() const
{
    return FloatRect(0.f, 0.f,
                     std::abs((float) m_textureRect.getWidth()),
                     std::abs((float) m_textureRect.getHeight()));
}

FloatRect Sprite::getGlobalBounds() const
{
    return FloatRect(m_position.x - m_origin.x * m_scale.x,
                     m_position.y - m_origin.y * m_scale.y,
                     std::abs((float) m_textureRect.getWidth()) * m_scale.x,
                     std::abs((float) m_textureRect.getHeight()) * m_scale.y);
}
