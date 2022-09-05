#ifndef RPG_SRC_CLIENT_GRAPHICS_OPENGL_UNIFORMBUFFER_H
#define RPG_SRC_CLIENT_GRAPHICS_OPENGL_UNIFORMBUFFER_H

#include <type_traits>
#include "glad/gl.h"
#include "../Uniforms.h"

template<typename T, typename ...Args>
constexpr bool isUniformTypeV = ((std::is_same_v<T, Args>) || ...);

template<typename T>
class UniformBuffer
{
private:
    static_assert(isUniformTypeV<T, LightUniform, MVPUniform>, "");

    unsigned int m_id{};
    unsigned int m_target{};
    bool m_isInit;
public:
    using UBOType = T;

    UniformBuffer();

    void init();

    void bind() const;
    void unbind() const;
    void destroy();

    void setSubData(T &&data) const;
};

template<typename T>
UniformBuffer<T>::UniformBuffer() : m_target(GL_UNIFORM_BUFFER), m_isInit(false) { }

template<typename T>
void UniformBuffer<T>::init()
{
    glCreateBuffers(1, &m_id);
    bind();
    glNamedBufferData(m_id, static_cast<GLsizeiptr>(sizeof(T)), nullptr, GL_STATIC_DRAW);
    unbind();
    m_isInit = true;
}

template<typename T>
void UniformBuffer<T>::bind() const
{
    glBindBuffer(m_target, m_id);
}

template<typename T>
void UniformBuffer<T>::unbind() const
{
    glBindBuffer(m_target, 0);
}

template<typename T>
void UniformBuffer<T>::destroy()
{
    if (!m_isInit) return;

    glDeleteBuffers(1, &m_id);
    m_isInit = false;
}

template<typename T>
void UniformBuffer<T>::setSubData(T &&data) const
{
    glNamedBufferSubData(m_id, 0, sizeof(T), &data);
}

using MVPUniformBuffer = UniformBuffer<MVPUniform>;
using LightUniformBuffer = UniformBuffer<LightUniform>;

#endif // RPG_SRC_CLIENT_GRAPHICS_OPENGL_UNIFORMBUFFER_H
