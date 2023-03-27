#ifndef RPG_VERTEX_H
#define RPG_VERTEX_H

#include <glm/glm.hpp>

struct Vertex
{
    glm::vec2 position;
    glm::vec4 color;
    glm::vec2 texCoord;
    float texId;
};

#endif // RPG_VERTEX_H
