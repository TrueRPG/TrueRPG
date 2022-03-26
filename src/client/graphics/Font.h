#ifndef RPG_FONT_H
#define RPG_FONT_H

#include <string>
#include <glm/glm.hpp>
#include <map>
#include "SpriteBatch.h"

struct Character {
    glm::ivec2 size; // The size of the character
    int xOffset; // The offset to the character
    int baseline;
};

class Font
{
    std::string m_path;
    int m_size; // The size of the font
    Texture m_texture;

    // Character map
    std::map<char, Character> m_characters;

    std::vector<unsigned char> m_pixelBuffer;

public:
    Font() = default;
    Font(const std::string& path, int size);

    std::string getPath() const;

    int getSize() const;

    void destroy();

private:
    Texture& getTexture();

    Character getCharacter(char c);

    void fillPixelBuffer(const unsigned char* buffer, size_t width, size_t height);

    friend class Text;
};

#endif //RPG_FONT_H
