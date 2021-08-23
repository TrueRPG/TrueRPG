#include "Font.h"

#include <ft2build.h>
#include FT_FREETYPE_H

Font::Font(const std::string &path, int size)
        : m_path(path), m_size(size) { }

Character Font::getCharacter(char c)
{
    return m_characters[c];
}

std::string Font::getPath() const
{
    return m_path;
}

int Font::getSize() const
{
    return m_size;
}

Texture &Font::getTexture()
{
    return m_texture;
}

void Font::destroy()
{
    m_texture.destroy();
}

// Выглядит жутко, но похожую штуку увидал в SFML. У нас особо нет выбора,
// потому что хочется переиспользовать уже имеющийся шейдер, а freetype умеет только в один канал.
void Font::fillPixelBuffer(const unsigned char *buffer, size_t width, size_t height)
{
    m_pixelBuffer.resize(width * height * 4);
    for (unsigned int y = 0; y < height; ++y)
    {
        for (unsigned int x = 0; x < width; ++x)
        {
            // Делаем белый цвет по умолчанию, альфу забиваем тем, что нам дал freetype
            std::size_t index = x + y * width;
            m_pixelBuffer[index * 4 + 0] = 255;
            m_pixelBuffer[index * 4 + 1] = 255;
            m_pixelBuffer[index * 4 + 2] = 255;
            m_pixelBuffer[index * 4 + 3] = buffer[index];
        }
    }
}

Font Font::create(const std::string& path, int size)
{
    Font res{path, size};

    FT_Library freetype;
    if (FT_Init_FreeType(&freetype))
    {
        std::cout << "Could not init FreeType Library" << std::endl;
        return Font();
    }

    FT_Face face;
    if (FT_New_Face(freetype, path.c_str(), 0, &face))
    {
        std::cout << "Failed to load font " << path << std::endl;
        return Font();
    }
    FT_Set_Pixel_Sizes(face, 0, size);

    // Вычисляем размеры атласа с глифами
    FT_GlyphSlot glyph = face->glyph;
    int width = 0;
    int height = 0;

    for (int i = 32; i < 128; i++)
    {
        if (FT_Load_Char(face, i, FT_LOAD_RENDER))
        {
            fprintf(stderr, "Loading character %c failed!\n", i);
            continue;
        }

        width += (int) glyph->bitmap.width;
        height = std::max(height, (int) glyph->bitmap.rows);
    }

    // Так как мы узнали ширину и высоту, то можем заранее создать текстуру для всех глифов
    unsigned int textureId;
    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    glTextureParameteri(textureId, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTextureParameteri(textureId, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTextureParameteri(textureId, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTextureParameteri(textureId, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTextureStorage2D(textureId, 1, GL_RGBA8, width, height);

    // Загружаем глифы в текстуру
    int x = 0;
    for (int i = 32; i < 128; i++)
    {
        if (FT_Load_Char(face, i, FT_LOAD_RENDER) || !glyph->bitmap.buffer)
        {
            // Иногда могут не загрузиться какие-то глифы (например, такое может быть на винде),
            // поэтому просто пропускаем такие случаи.
            continue;
        }

        res.fillPixelBuffer(glyph->bitmap.buffer, glyph->bitmap.width, glyph->bitmap.rows);

        // уOffset нужен, чтобы разместить наши символы на одной линии
        glTextureSubImage2D(textureId, 0, x, 0, glyph->bitmap.width, glyph->bitmap.rows,
                            GL_RGBA, GL_UNSIGNED_BYTE, &res.m_pixelBuffer[0]);

        int baseline = height - glyph->bitmap_top;
        Character character = {glm::ivec2(glyph->bitmap.width, glyph->bitmap.rows), x, baseline};
        res.m_characters.insert({i, character});

        x += glyph->bitmap.width;
    }

    res.m_texture = Texture(textureId, path, width, height);

    // Уничтожаем все это безобразие
    FT_Done_Face(face);
    FT_Done_FreeType(freetype);

    return res;
}
