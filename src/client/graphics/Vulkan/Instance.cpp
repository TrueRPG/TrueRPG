#include "../../../pch.h"
#include "Instance.h"
#include "../../Engine.h"

namespace vk
{

    namespace error
    {
        struct SurfaceErrorCategory : std::error_category
        {
            [[nodiscard]] const char *name() const noexcept override { return "vk_surface"; }
            [[nodiscard]] std::string message(int err) const override { return "vk_surface: failed initialization"; }
        };

        static const SurfaceErrorCategory surfaceErrorCategory;

        Error<> makeError()
        {
            return {{1, surfaceErrorCategory}};
        }
    }

Instance::Instance(VkInstance value, VkPhysicalDevice physicalDevice, VkDebugUtilsMessengerEXT messengerExt)
    : m_instance(value),
      m_physicalDevice(physicalDevice)
#ifdef DEBUG
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
        return ::Result<Surface>(error::makeError());

    return ::Result<Surface>(surfaceKhr, m_physicalDevice);
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