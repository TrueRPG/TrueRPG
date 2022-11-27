#include "AllocatorBuilder.h"

#include "../Device.h"

namespace vk
{

AllocatorBuilder &AllocatorBuilder::setApiVersion(u32 version)
{
    m_info.apiVersion = version;
    return *this;
}

AllocatorBuilder &AllocatorBuilder::setDevice(const Device &device)
{
    m_info.device = &device;
    return *this;
}

AllocatorBuilder &AllocatorBuilder::setInstance(VkInstance instance)
{
    m_info.instance = instance;
    return *this;
}

ObjResult<Allocator> AllocatorBuilder::build() const
{
    VmaAllocator allocator = VK_NULL_HANDLE;
    VmaVulkanFunctions vulkanFunctions{};
    vulkanFunctions.vkGetInstanceProcAddr = &vkGetInstanceProcAddr;
    vulkanFunctions.vkGetDeviceProcAddr = vkGetDeviceProcAddr;

    VmaAllocatorCreateInfo info{};
    info.vulkanApiVersion = m_info.apiVersion;
    info.physicalDevice = m_info.device->getPhysicalDevice();
    info.device = *m_info.device;
    info.instance = m_info.instance;
    info.pVulkanFunctions = &vulkanFunctions;

    VkResult result = vmaCreateAllocator(&info, &allocator);
    if (result != VK_SUCCESS)
        throw;

    return vk::ObjResult<Allocator>(allocator);
}

}