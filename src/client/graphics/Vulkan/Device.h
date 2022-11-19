//
// Created by archweeb on 10/7/22.
//

#ifndef RPG_DEVICE_H
#define RPG_DEVICE_H

#include <vulkan/vulkan.h>

namespace vk
{

class Device
{
private:
    VkDevice m_device = VK_NULL_HANDLE;
    VkPhysicalDevice m_physicalDevice = VK_NULL_HANDLE;
public:
};

} // namespace vk

#endif // RPG_DEVICE_H
