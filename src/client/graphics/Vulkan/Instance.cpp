#include "../../../pch.h"
#include "Instance.h"
#include "../../Engine.h"

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

VkPhysicalDevice Instance::getPhysicalDevice() const
{
    return m_physicalDevice;
}

::Result<Surface> Instance::createSurface() const
{
    Surface surfaceKhr;
    if  (!Engine::getWindow().createSurface(*this, surfaceKhr))
        return {};

    return ::Result<Surface>(surfaceKhr);
}

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