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
#include "Sprite.h"

struct Vertex
{
    glm::vec3 position;
    glm::vec4 color;
    glm::vec2 texCoord;
    float texId;
};

struct VertexWrapper
{
    Vertex vertex{};
    int order{0};
};

static const size_t MaxTextures = 16;

class SpriteBatch
{
    Shader m_shader;

    int m_maxSprites;
    VertexArray m_vao;
    Buffer m_vbo;

    Buffer m_ibo;

    inline static bool compareVertices(const VertexWrapper &a, const VertexWrapper &b)
    {
        return a.order < b.order;
    }

    // layer -> vertices
    std::map<int, std::multiset<VertexWrapper, decltype(&compareVertices)>> m_vertices;
    int m_spritesSize{0};

    Texture m_textures[MaxTextures];
    int m_texturesSize{0};

    // It's not necessary to have these fields here,
    // but it's quite useful for the rendering system
    glm::mat4 m_projMat{};
    glm::mat4 m_viewMat{};

public:
    SpriteBatch() = default;

    SpriteBatch(Shader shader, int spriteCount = 2000);

    void begin();

    void end();

    void draw(const Sprite &sprite, int layer = 0, int order = 0);

    glm::mat4 getProjectionMatrix();

    void setProjectionMatrix(glm::mat4 projMat);

    glm::mat4 getViewMatrix();

    void setViewMatrix(glm::mat4 viewMat);

    Shader getShader();

    void destroy();

};

#endif //RPG_SPRITEBATCH_H
