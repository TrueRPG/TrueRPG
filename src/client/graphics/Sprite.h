#ifndef RPG_SPRITE_H
#define RPG_SPRITE_H

#include <glm/glm.hpp>
#include "Rect.h"
#include "Texture.h"

class Sprite
{
    glm::vec2 m_position{};
    glm::vec2 m_origin{};
    glm::vec2 m_scale{1.f};
    glm::vec4 m_color{1.f};
    IntRect m_textureRect{0, 0, 0, 0};
    Texture* m_texture;

public:
    Sprite() = default;
    Sprite(Texture& texture);

    glm::vec2 getPosition() const;
    void setPosition(glm::vec2 position);

    glm::vec2 getOrigin() const;
    void setOrigin(glm::vec2 origin);

    glm::vec2 getScale() const;
    void setScale(glm::vec2 scale);

    glm::vec4 getColor() const;
    void setColor(glm::vec4 color);

    Texture& getTexture() const;

    IntRect getTextureRect() const;
    void setTextureRect(const IntRect &rect);

    /**
     * Get the bounds of the sprite in the local coordinates.
     *
     * @return the local bounds of the sprite
     */
    FloatRect getLocalBounds() const;

    /**
     * Get the bounds of the sprite in the world coordinates.
     *
     * @return the global bounds of the sprite
     */
    FloatRect getGlobalBounds() const;
};

#endif //RPG_SPRITE_H
