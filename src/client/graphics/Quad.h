#ifndef RPG_SRC_CLIENT_GRAPHICS_QUAD_H
#define RPG_SRC_CLIENT_GRAPHICS_QUAD_H

#include "VertexArray.h"
#include "Buffer.h"

class Quad
{
private:
    VertexArray m_vao;
    Buffer m_vbo;
public:
    Quad();

    void draw();

    void destroy();
};

#endif // RPG_SRC_CLIENT_GRAPHICS_QUAD_H
