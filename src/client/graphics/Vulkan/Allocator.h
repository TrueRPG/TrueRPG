#ifndef RPG_ALLOCATOR_H
#define RPG_ALLOCATOR_H

#include <vulkan/vulkan.h>
#include <vk_mem_alloc.h>

namespace vk
{

class Allocator
{
private:
    VmaAllocator m_allocator = VK_NULL_HANDLE;

public:
    Allocator() = default;
    explicit Allocator(VmaAllocator allocator);

    void destroy();

    operator VmaAllocator() const;
};

}

#endif // RPG_ALLOCATOR_H
