#ifndef RPG_FONT_H
#define RPG_FONT_H

#include <string>
#include <glm/glm.hpp>
#include <map>
#include "SpriteBatch.h"

struct Character {
    glm::ivec2 size; // Размер символа
    int xOffset; // Смещение от начала текстуры, до символа
    int baseline;
};

class Font
{
    std::string m_path;
    int m_size; // Размер шрифта
    Texture m_texture;

    // Мапа с символами
    std::map<char, Character> m_characters;

    std::vector<unsigned char> m_pixelBuffer;

public:
    Font() = default;
    Font(const std::string& path, int size);

    std::string getPath() const;

    int getSize() const;

    void destroy();

    static Font create(const std::string& path, int size);
private:
    Texture& getTexture();

    Character getCharacter(char c);

    void fillPixelBuffer(const unsigned char* buffer, size_t width, size_t height);

    friend class Text;
};

#endif //RPG_FONT_H
