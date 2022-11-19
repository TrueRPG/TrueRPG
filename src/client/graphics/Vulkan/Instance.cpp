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

} // namespace vk