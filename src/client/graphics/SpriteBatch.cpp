#include "../../pch.h"
#include "SpriteBatch.h"

#include "Graphics.h"
#include <numeric>

SpriteBatch::SpriteBatch(Shader shader, int maxSprites)
        : m_shader(shader), m_maxSprites(maxSprites),
          m_vbo(GL_ARRAY_BUFFER), m_ibo(GL_ELEMENT_ARRAY_BUFFER)
{
    const int vertexCount = maxSprites * 4;
    const int indexCount = maxSprites * 6;

    m_vao.bind();
    m_vbo.bind();

    // This is a little trick.
    // Instead of putting the data into vbo, we just allocate memory for later use
    m_vbo.setData(nullptr, sizeof(Vertex) * vertexCount, GL_DYNAMIC_DRAW);

    // Coords
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *) 0);
    glEnableVertexAttribArray(0);

    // Color
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *) (3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Texture coords
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *) (7 * sizeof(float)));
    glEnableVertexAttribArray(2);

    // Texture index
    glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *) (9 * sizeof(float)));
    glEnableVertexAttribArray(3);

    // Pattern:
    // 0, 1, 2, 2, 3, 0
    // 4, 5, 6, 6, 7, 4
    // etc.
    auto *indices = new unsigned int[indexCount];
    unsigned int offset = 0;
    for (int i = 0; i < indexCount; i += 6)
    {
        indices[i + 0] = 0 + offset;
        indices[i + 1] = 1 + offset;
        indices[i + 2] = 2 + offset;

        indices[i + 3] = 2 + offset;
        indices[i + 4] = 3 + offset;
        indices[i + 5] = 0 + offset;

        offset += 4;
    };
    m_ibo.bind();
    m_ibo.setData(indices, indexCount * sizeof(unsigned int), GL_STATIC_DRAW);

    m_vbo.unbind();
    m_vao.unbind();
    delete[] indices;
}

void SpriteBatch::begin()
{
    m_vertices.clear();
    m_spritesSize = 0;
    m_texturesSize = 0;
    m_shader.use();
}

void SpriteBatch::end()
{
    // In this method we draw all vertices at once with a single draw call
    m_vbo.bind();

    // Insert everything into one vector
    std::vector<Vertex> vertices;

    for (const auto &layer : m_vertices)
    {
        for (const auto &vertex : layer.second)
        {
            vertices.push_back(vertex.vertex);
        }
    }

    // Put our vertices into the allocated memory
    m_vbo.setSubData(vertices, 0);

    m_shader.use();

    int *ids = new int[m_texturesSize];
    std::iota(ids, ids + m_texturesSize, 0);
    m_shader.setUniform("textures", m_texturesSize, ids);

    m_shader.setUniform("model", glm::mat4(1));

    for (int i = 0; i < m_texturesSize; i++)
    {
        m_textures[i].bind(i);
    }

    m_vao.bind();
    glDrawElements(GL_TRIANGLES, m_spritesSize * 6, GL_UNSIGNED_INT, nullptr);

    delete[] ids;
}

// This function makes our rect a bit smaller.
// It helps to prevent strange artifacts with textures.
static FloatRect prepareRect(IntRect rect)
{
    float offset = 0.5f;

    auto left = (float) rect.getLeft();
    auto bottom = (float) rect.getBottom();
    auto width = (float) rect.getWidth();
    auto height = (float) rect.getHeight();

    left += glm::sign(width) * offset;
    bottom += glm::sign(height) * offset;

    width = glm::sign(width) * (std::abs(width) - 2 * offset);
    height = glm::sign(height) * (std::abs(height) - 2 * offset);

    return {left, bottom, width, height};
}

static glm::vec2 toTexCoords(Texture& texture, float x, float y)
{
    return {(float) x / texture.getWidth(), (float) y / texture.getHeight()};
}

void SpriteBatch::draw(const Sprite &sprite, int layer, int order)
{
    // Actually we draw nothing here. In this method we just collect the sprites to draw them later
    if (m_spritesSize >= m_maxSprites)
    {
        std::cerr << "Cannot draw a sprite! Maximum number of sprites reached!" << std::endl;
        return;
    }
    m_spritesSize++;

    glm::vec2 quadPos = sprite.getPosition() - sprite.getOrigin() * sprite.getScale();
    IntRect rect = sprite.getTextureRect();

    Texture texture = sprite.getTexture();

    int i;
    for (i = 0; i < m_texturesSize; i++)
    {
        if (m_textures[i].getId() == texture.getId())
        {
            break;
        }
    }

    if (i >= MaxTextures)
    {
        std::cerr << "Cannot draw a sprite with texture " << texture.getPath()
                  << "! Maximum number of m_texturesSize reached!" << std::endl;
        return;
    }

    // If we get to the end, add a new texture
    if (i == m_texturesSize)
    {
        m_textures[i] = texture;
        m_texturesSize++;
    }

    auto texId = static_cast<float>(i);

    float w = (float) std::abs(rect.getWidth()) * sprite.getScale().x;
    float h = (float) std::abs(rect.getHeight()) * sprite.getScale().y;

    // Create a layer if absent
    auto resultSet = m_vertices.find(layer);
    if (resultSet == m_vertices.end())
    {
        std::multiset<VertexWrapper, decltype(&compareVertices)> set(compareVertices);
        resultSet = m_vertices.insert({layer, set}).first;
    }
    auto &set = resultSet->second;

    FloatRect r = prepareRect(rect);

    set.insert(
            {
                    {
                            glm::vec3(quadPos, 0.f), // bottom left
                            sprite.getColor(),
                            toTexCoords(texture, r.getLeft(), r.getBottom()), texId
                    }, order
            });
    set.insert(
            {
                    {
                            glm::vec3(quadPos + glm::vec2(w, 0.f), 0.f), // bottom right
                            sprite.getColor(),
                            toTexCoords(texture, r.getLeft() + r.getWidth(), r.getBottom()),
                            texId
                    }, order
            });
    set.insert(
            {
                    {
                            glm::vec3(quadPos + glm::vec2(w, h), 0.f), // top right
                            sprite.getColor(),
                            toTexCoords(texture, r.getLeft() + r.getWidth(), r.getBottom() + r.getHeight()),
                            texId
                    }, order
            });
    set.insert(
            {
                    {
                            glm::vec3(quadPos + glm::vec2(0.f, h), 0.f), // top left
                            sprite.getColor(),
                            toTexCoords(texture, r.getLeft(), r.getBottom() + r.getHeight()),
                            texId
                    }, order
            });
}

void SpriteBatch::setShader(Shader shader)
{
    m_shader = shader;
}

glm::mat4 SpriteBatch::getProjectionMatrix()
{
    return m_projMat;
}

void SpriteBatch::setProjectionMatrix(glm::mat4 projMat)
{
    m_projMat = projMat;
    m_shader.use();
    m_shader.setUniform("projection", projMat);
}

glm::mat4 SpriteBatch::getViewMatrix()
{
    return m_viewMat;
}

void SpriteBatch::setViewMatrix(glm::mat4 viewMat)
{
    m_viewMat = viewMat;
    m_shader.use();
    m_shader.setUniform("view", viewMat);
}
void SpriteBatch::destroy()
{
    m_vao.destroy();
    m_vbo.destroy();
    m_ibo.destroy();
}
