#ifndef RPG_ALLOCATORBUILDER_H
#define RPG_ALLOCATORBUILDER_H

#include <vulkan/vulkan.h>
#include <vk_mem_alloc.h>
#include "../VulkanUtils.h"
#include "../Allocator.h"

namespace vk
{
class Device;

class AllocatorBuilder
{
private:
    struct Info
    {
        u32 apiVersion = VK_API_VERSION_1_2;
        const Device *device = nullptr;
        VkInstance instance = VK_NULL_HANDLE;
    } m_info;
public:
    AllocatorBuilder &setApiVersion(u32 version);
    AllocatorBuilder &setDevice(const Device &device);
    AllocatorBuilder &setInstance(VkInstance instance);

    ObjResult<Allocator> build() const;
};

}

#endif // RPG_ALLOCATORBUILDER_H
