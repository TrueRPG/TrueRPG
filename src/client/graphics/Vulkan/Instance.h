#ifndef RPG_INSTANCE_H
#define RPG_INSTANCE_H

#include <vulkan/vulkan.h>
#include "VulkanUtils.h"
#include "Device.h"

namespace vk
{
class Instance
{
private:
    VkInstance m_instance = VK_NULL_HANDLE;
#ifdef VK_DEBUG
    VkDebugUtilsMessengerEXT m_messanger = VK_NULL_HANDLE;
#endif
    VkPhysicalDevice m_physicalDevice = VK_NULL_HANDLE;
    friend class InstanceBuilder;
public:
    Instance() = default;
    explicit Instance(VkInstance value);
    Instance(VkInstance value, VkPhysicalDevice physicalDevice);
private:
};

}

#endif