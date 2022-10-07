#ifndef RPG_SPRITEBATCH_H
#define RPG_SPRITEBATCH_H

#include <glm/glm.hpp>
#include <vector>
#include <map>
#include <set>
#include "VertexArray.h"
#include "Buffer.h"
#include "Texture.h"
#include "Shader.h"
#include "../Sprite.h"

struct Vertex
{
    glm::vec2 position;
    glm::vec4 color;
    glm::vec2 texCoord;
    float texId;
};

struct ShortVertex
{
    glm::vec2 position;
    glm::vec2 texCoords;
};

struct QuadWrapper
{
    ShortVertex vertices[4];
    glm::vec4 color;
    float texId;
    int order{0};
};

static const size_t MaxTextures = 16;
static const size_t MaxLayers = 16;

class SpriteBatch
{
    IShader *m_shader;

    int m_maxSprites{0};
    VertexArray m_vao;
    Buffer m_vbo;

    Buffer m_ibo;

    struct QuadComparator
    {
        bool operator()(const QuadWrapper &a, const QuadWrapper &b) const
        {
            return a.order < b.order;
        }
    };

    std::multiset<QuadWrapper, QuadComparator> m_layers[MaxLayers];

    int m_spritesSize{0};

    Texture m_textures[MaxTextures];
    int m_texturesSize{0};

    // It's not necessary to have these fields here,
    // but it's quite useful for the rendering system
    glm::mat4 m_projMat{};
    glm::mat4 m_viewMat{};

public:
    SpriteBatch() = default;

    SpriteBatch(IShader *shader, int spriteCount = 2000);

    void begin();

    void end();

    void draw(const Sprite &sprite, int layer = 0, int order = 0);

    void setShader(IShader *shader);

    glm::mat4 getProjectionMatrix();

    void setProjectionMatrix(glm::mat4 projMat);

    glm::mat4 getViewMatrix();

    void setViewMatrix(glm::mat4 viewMat);

    void destroy();

};

#endif //RPG_SPRITEBATCH_H
