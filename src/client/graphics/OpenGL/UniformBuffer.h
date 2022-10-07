#ifndef RPG_SRC_CLIENT_GRAPHICS_OPENGL_UNIFORMBUFFER_H
#define RPG_SRC_CLIENT_GRAPHICS_OPENGL_UNIFORMBUFFER_H

#include <type_traits>
#include <glad/gl.h>
#include "../Uniforms.h"
#include "../../../utils/Types.h"

template<typename T, typename ...Args>
constexpr bool IsSameTypeV = ((std::is_same_v<T, Args>) || ...);

template<typename T>
constexpr bool IsUnformTypeV = IsSameTypeV<T, LightUniform, MVPUniform, GlobalLightUniform>;

class IUniformBuffer
{
public:
    virtual ~IUniformBuffer() = default;

    virtual void init() = 0;
    virtual void bind() const = 0;
    virtual void bindBase(i32 index = 0) const = 0;
    virtual void unbind() const = 0;
    virtual void destroy() = 0;
    virtual void setSubData(void *data) = 0;
};

template<typename T>
class UniformBuffer : public IUniformBuffer
{
private:
    static_assert(IsUnformTypeV<T>, "");

    unsigned int m_id{};
    static constexpr unsigned int TARGET = GL_UNIFORM_BUFFER;
    bool m_isInit;
public:
    using UBOType = T;

    UniformBuffer();

    void init() override;

    void bind() const override;
    void bindBase(i32 index = 0) const override;
    void unbind() const override;
    void destroy() override;

    void setSubData(void *data) override;
    void setSubData(T &data);
};

template<typename T>
UniformBuffer<T>::UniformBuffer() : m_isInit(false) { }

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
    glBindBuffer(TARGET, m_id);
}

template<typename T>
void UniformBuffer<T>::bindBase(i32 index) const
{
    glBindBufferBase(TARGET, index, m_id);
}

template<typename T>
void UniformBuffer<T>::unbind() const
{
    glBindBuffer(TARGET, 0);
}

template<typename T>
void UniformBuffer<T>::destroy()
{
    if (!m_isInit) return;

    glDeleteBuffers(1, &m_id);
    m_isInit = false;
}

template<typename T>
void UniformBuffer<T>::setSubData(T &data)
{
    glNamedBufferSubData(m_id, 0, sizeof(T), &data);
}

template<typename T>
void UniformBuffer<T>::setSubData(void *data)
{
    setSubData(*reinterpret_cast<T *>(data));
}

using MVPUniformBuffer = UniformBuffer<MVPUniform>;
using LightUniformBuffer = UniformBuffer<LightUniform>;
using GlobalLightBuffer = UniformBuffer<GlobalLightUniform>;

#endif // RPG_SRC_CLIENT_GRAPHICS_OPENGL_UNIFORMBUFFER_H
