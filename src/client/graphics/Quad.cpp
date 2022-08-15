#include "../../pch.h"
#include "Quad.h"

Quad::Quad()
    : m_vao(),
      m_vbo(GL_ARRAY_BUFFER)
{
    static const float quad_vertices[] = {
        -1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
        -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
        1.0f, 1.0f, 0.0f, 1.0f, 1.0f,
        1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
    };
    m_vao.bind();
    m_vbo.bind();
    m_vbo.setData(quad_vertices, sizeof(quad_vertices), GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *) 0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *) (3 * sizeof(float)));
    m_vbo.unbind();
    m_vao.unbind();
}

void Quad::draw()
{
    m_vao.bind();
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    m_vao.unbind();
}

void Quad::destroy()
{
    m_vbo.destroy();
    m_vao.destroy();
}
