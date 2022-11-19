#include "../../../pch.h"
#include "Instance.h"

namespace vk
{

Instance::Instance(VkInstance value, VkPhysicalDevice physicalDevice, VkDebugUtilsMessengerEXT messengerExt)
    : m_instance(value),
      m_physicalDevice(physicalDevice)
#ifdef VK_DEBUG
      ,
      m_messanger(messengerExt)
#endif
{}

void Instance::destroy()
{
    destroyDebugUtilsMessengerEXT(m_instance, m_messanger, nullptr);
    vkDestroyInstance(m_instance, nullptr);
}

Instance::operator VkInstance() const
{
    return m_instance;
}

} // namespace vk