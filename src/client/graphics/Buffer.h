#ifndef RPG_BUFFER_H
#define RPG_BUFFER_H

#include <cstddef>
#include <vector>
#include "IGLObject.h"
#include "Graphics.h"

class Buffer : public IGLObject
{
private:
    unsigned int m_id{};
    unsigned int m_target{};
public:
    Buffer() = default;
    Buffer(unsigned int target);

    void bind() const;
    void unbind() const;
    void destroy() override;

    void setData(const void* data, size_t size, unsigned int usage) const;
    void setSubData(const void* data, GLintptr offset, size_t size) const;

    template<typename T>
    void setData(const std::vector<T>& data, unsigned int usage) const
    {
        glNamedBufferData(m_id, data.size() * sizeof(T), data.data(), usage);
    }
    
    template<typename T>
    void setSubData(const std::vector<T>& data, GLintptr offset)
    {
        glNamedBufferSubData(m_id, offset, data.size() * sizeof(T), data.data());
    }

    unsigned int getId() const override;
    unsigned int getTarget() const;
};

#endif //RPG_BUFFER_H