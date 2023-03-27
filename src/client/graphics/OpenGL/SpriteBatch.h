#ifndef RPG_SPRITEBATCH_H
#define RPG_SPRITEBATCH_H

#include <glm/glm.hpp>
#include <vector>
#include <map>
#include <set>
#include "../Vertex.h"
#include "VertexArray.h"
#include "Buffer.h"
#include "Texture.h"
#include "Shader.h"
#include "../Sprite.h"
#include "../ISpriteBatch.h"

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

class SpriteBatch : public ISpriteBatch
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

    ITexture *m_textures[MaxTextures];
    int m_texturesSize{0};

    // It's not necessary to have these fields here,
    // but it's quite useful for the rendering system
    glm::mat4 m_projMat{};
    glm::mat4 m_viewMat{};

public:
    SpriteBatch() = default;

    SpriteBatch(IShader *shader, int spriteCount = 2000);

    void begin() override;

    void end() override;

    void draw(const Sprite &sprite, int layer = 0, int order = 0) override;

    void setShader(IShader *shader) override;

    glm::mat4 getProjectionMatrix() override;

    void setProjectionMatrix(glm::mat4 projMat) override;

    glm::mat4 getViewMatrix() override;

    void setViewMatrix(glm::mat4 viewMat) override;

    void destroy() override;

};

#endif //RPG_SPRITEBATCH_H
