#ifndef RPG_SURFACE_H
#define RPG_SURFACE_H

#include <vulkan/vulkan.h>

namespace vk
{

struct Surface
{
    VkSurfaceKHR surface = VK_NULL_HANDLE;
    VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;

    inline Surface() = default;
    inline Surface(VkSurfaceKHR _surface, VkPhysicalDevice _physicalDevice)
        : surface(_surface),
          physicalDevice(_physicalDevice)
    {}

    inline VkSurfaceCapabilitiesKHR getCapabilities() const
    {
        VkSurfaceCapabilitiesKHR capabilities;
        vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice, surface, &capabilities);
        return capabilities;
    }

    inline std::vector<VkPresentModeKHR> getPresentModes() const
    {
        static std::vector<VkPresentModeKHR> presentModes;
        if (presentModes.empty())
        {
            u32 presentModesCount = 0;
            vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &presentModesCount, nullptr);
            presentModes.resize(presentModesCount);
            vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &presentModesCount, presentModes.data());
        }

        return presentModes;
    }

    inline std::vector<VkSurfaceFormatKHR> getFormats() const
    {
        static std::vector<VkSurfaceFormatKHR> surfaceFormats;

        if (surfaceFormats.empty())
        {
            u32 formatsCount = 0;
            vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &formatsCount, nullptr);
            surfaceFormats.resize(formatsCount);
            vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &formatsCount, surfaceFormats.data());
        }

        return surfaceFormats;
    }

    inline operator VkSurfaceKHR() const { return surface; }
};

} // namespace vk

#endif // RPG_SURFACE_H
