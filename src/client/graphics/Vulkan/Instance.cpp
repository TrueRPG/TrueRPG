#include "Instance.h"

namespace vk
{

Instance::Instance(VkInstance value) : m_instance(value), m_physicalDevice(VK_NULL_HANDLE) {}
Instance::Instance(VkInstance value, VkPhysicalDevice physicalDevice) : m_instance(value), m_physicalDevice(physicalDevice) {}

} // namespace vk