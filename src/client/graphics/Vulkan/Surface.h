#ifndef RPG_SURFACE_H
#define RPG_SURFACE_H

#include <vulkan/vulkan.h>

namespace vk
{

struct Surface
{
    VkSurfaceKHR surface = VK_NULL_HANDLE;

    inline Surface() = default;
    inline explicit Surface(VkSurfaceKHR _surface) : surface(_surface)
    {}

    inline operator VkSurfaceKHR() const { return surface; }
};

} // namespace vk

#endif // RPG_SURFACE_H
