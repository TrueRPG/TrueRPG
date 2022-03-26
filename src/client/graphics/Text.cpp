#include "Text.h"

Text::Text(Font &font, std::string text)
        : m_font(font), m_scale(1.f),
          m_color(1.f), m_text(std::move(text))
{
    initSprites();
}

void Text::draw(SpriteBatch &batch, int layer, int order)
{
    for (auto sprite : m_sprites)
    {
        // Do some magic to make the center in the bottom left corner and to make the origin works
        sprite.setPosition(m_position + (sprite.getPosition() + glm::vec2(0.f, m_height) - m_origin) * m_scale);
        sprite.setScale(m_scale);
        sprite.setColor(m_color);
        batch.draw(sprite, layer, order);
    }
}

std::string Text::getText() const
{
    return m_text;
}

// Please note, that the size depends on the text
void Text::setText(const std::string &text)
{
    m_text = text;
    initSprites();
}

glm::vec2 Text::getPosition() const
{
    return m_position;
}

void Text::setPosition(glm::vec2 position)
{
    m_position = position;
}

glm::vec2 Text::getOrigin() const
{
    return m_origin;
}

void Text::setOrigin(glm::vec2 origin)
{
    m_origin = origin;
}

glm::vec2 Text::getScale() const
{
    return m_scale;
}

void Text::setScale(glm::vec2 scale)
{
    m_scale = scale;
}

glm::vec4 Text::getColor() const
{
    return m_color;
}

void Text::setColor(glm::vec4 color)
{
    m_color = color;
}

FloatRect Text::getLocalBounds() const
{
    return FloatRect(0.f, 0.f, m_width, m_height);
}

FloatRect Text::getGlobalBounds() const
{
    return FloatRect(m_position.x - m_origin.x * m_scale.x,
                     m_position.y - m_origin.y * m_scale.y,
                     m_width * m_scale.x,
                     m_width * m_scale.y);
}

void Text::initSprites()
{
    m_sprites.clear();
    Sprite sprite(m_font.getTexture());

    // Find out the size of the text
    float maxWidth = 0;
    float maxHeight = 0;

    glm::vec2 pos(0.f);
    for (char c : m_text)
    {
        Character character = m_font.getCharacter(c);

        if (c == ' ')
        {
            // Let's make the space size dependent on the font size
            pos += glm::vec2(m_font.getSize() / 4, 0.f);
            continue;
        }
        if (c == '\n')
        {
            // Do magic to make the indents, compute the width and height. Looks scary...
            maxWidth = std::max(maxWidth, pos.x);
            maxHeight += (float) m_font.getSize();
            pos = glm::vec2(0.f, pos.y - (float) m_font.getSize());
            continue;
        }

        // We have to mirror the texture, because the coordinates in freetype starts in the top left point,
        // but we need in the bottom left. If someone knows how to do it better, please drop me a message
        sprite.setTextureRect(
                IntRect(character.xOffset, character.size.y, character.size.x, -character.size.y)
        );
        // Don't ask me why the origin is like this
        // For some reason it was more convenient
        sprite.setOrigin(glm::vec2(0.f, character.size.y));
        sprite.setPosition(pos - glm::vec2(0.f, character.baseline));

        m_sprites.push_back(sprite);

        pos += glm::vec2((float) character.size.x, 0.f);
    }
    // Check the width for the last line again
    maxWidth = std::max(maxWidth, pos.x);
    // And add the height of the last line
    maxHeight += (float) m_font.getSize();

    m_width = maxWidth;
    m_height = maxHeight;
}
