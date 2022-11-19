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

}

#endif // RPG_VULKANUTILS_H
