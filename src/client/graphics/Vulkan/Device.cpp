#include "../../../pch.h"
#include "Device.h"

namespace vk
{
Device::Device(VkDevice device, VkPhysicalDevice physicalDevice)
    : m_device(device),
      m_physicalDevice(physicalDevice)
{}

Device::operator VkDevice() const
{
    return m_device;
}

void Device::destroy()
{
    vkDestroyDevice(m_device, nullptr);
}

} // namespace vk