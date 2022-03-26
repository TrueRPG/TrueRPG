#ifndef RPG_TEXT_H
#define RPG_TEXT_H

#include "Sprite.h"
#include "Font.h"

/**
 * This class was created to simplify text-related code.
 * You can use it to get the size of the text, change some parameters etc.
 */
class Text
{
    Font& m_font;
    std::string m_text;

    glm::vec2 m_position{};
    glm::vec2 m_origin{};
    glm::vec2 m_scale{};
    glm::vec4 m_color;

    float m_width;
    float m_height;

    std::vector<Sprite> m_sprites;

public:
    Text(Font& font, std::string text);

    void draw(SpriteBatch& batch, int layer = 0, int order = 0);

    std::string getText() const;
    void setText(const std::string& text);

    glm::vec2 getPosition() const;
    void setPosition(glm::vec2 position);

    glm::vec2 getOrigin() const;
    void setOrigin(glm::vec2 origin);

    glm::vec2 getScale() const;
    void setScale(glm::vec2 scale);

    glm::vec4 getColor() const;
    void setColor(glm::vec4 color);

    /**
     * Get the bounds of the text in the local coordinates.
     *
     * @return the local bounds of the text
     */
    FloatRect getLocalBounds() const;

    /**
     * Get the bounds of the text in the world coordinates.
     *
     * @return the global bounds of the text
     */
    FloatRect getGlobalBounds() const;

private:
    void initSprites();
};

#endif //RPG_TEXT_H
