//
// Created by archweeb on 10/7/22.
//

#ifndef RPG_DEVICE_H
#define RPG_DEVICE_H

#include <vulkan/vulkan.h>
#include "Queue.h"
#include "Swapchain.h"
#include "VulkanUtils.h"

namespace vk
{

class Surface;

class Device
{
private:
    VkDevice m_device = VK_NULL_HANDLE;
    VkPhysicalDevice m_physicalDevice = VK_NULL_HANDLE;

    Queue m_graphics;
    Queue m_presentation;
public:

    Device() = default;
    Device(VkDevice device, VkPhysicalDevice physicalDevice, Queue graphics, Queue presentation);

    VkPhysicalDevice getPhysicalDevice() const;
    [[nodiscard]] Queue getGraphicsQueue() const;
    [[nodiscard]] Queue getPresentationQueue() const;

    ObjResult<Swapchain> createSwapchain(const Surface &surface) const;

    void destroy();

    operator VkDevice() const;
};

} // namespace vk

#endif // RPG_DEVICE_H
