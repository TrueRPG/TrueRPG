#include "../../pch.h"
#include "Buffer.h"

Buffer::Buffer(unsigned int target) : m_target(target)
{
    glGenBuffers(1, &m_id);    
}

void Buffer::bind() const
{
    glBindBuffer(m_target, m_id);
}

void Buffer::unbind() const
{
    glBindBuffer(m_target, 0);
}

void Buffer::destroy()
{
    glDeleteBuffers(1, &m_id);
    m_id = 0;
    m_target = 0;
}

void Buffer::setData(const void* data, size_t size, unsigned int usage) const
{
    glBufferData(m_target, static_cast<GLsizeiptr>(size), data, usage);
}

void Buffer::setSubData(const void* data, GLintptr offset, size_t size) const
{
    glBufferSubData(m_target, offset, static_cast<GLsizeiptr>(size), data);
}


unsigned int Buffer::getId() const
{
    return m_id;
}

unsigned int Buffer::getTarget() const
{
    return m_target;
}
