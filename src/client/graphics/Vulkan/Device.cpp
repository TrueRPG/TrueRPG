#include "Device.h"
#include "../../../pch.h"

namespace vk
{
Device::Device(VkDevice device, VkPhysicalDevice physicalDevice)
    : m_device(device),
      m_physicalDevice(physicalDevice)
{}

} // namespace vk