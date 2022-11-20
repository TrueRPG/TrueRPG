#ifndef RPG_VULKANUTILS_H
#define RPG_VULKANUTILS_H

#include "../../../utils/Result.h"

namespace vk
{
#ifndef NDEBUG
#define VK_DEBUG
constexpr bool DEBUG_ENABLED = true;
#else
constexpr bool LOG_DEBUG_ENABLED = false;
#endif

inline void destroyDebugUtilsMessengerEXT(VkInstance instance,
    VkDebugUtilsMessengerEXT debugMessenger,
    const VkAllocationCallbacks* pAllocator)
{
    auto func = (PFN_vkDestroyDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
    if (func != nullptr)
        func(instance, debugMessenger, pAllocator);
}

template <typename T>
using Result = ::Result<T, Error<VkResult>>;

}

#endif // RPG_VULKANUTILS_H
