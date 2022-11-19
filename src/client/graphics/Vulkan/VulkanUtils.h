#ifndef RPG_VULKANUTILS_H
#define RPG_VULKANUTILS_H


namespace vk
{
#ifndef NDEBUG
#define VK_DEBUG
constexpr bool DEBUG_ENABLED = true;
#else
constexpr bool DEBUG_ENABLED = false;
#endif

inline void destroyDebugUtilsMessengerEXT(VkInstance instance,
    VkDebugUtilsMessengerEXT debugMessenger,
    const VkAllocationCallbacks* pAllocator)
{
    auto func = (PFN_vkDestroyDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
    if (func != nullptr)
        func(instance, debugMessenger, pAllocator);
}

}

#endif // RPG_VULKANUTILS_H
