#ifndef RPG_SWAPCHAIN_H
#define RPG_SWAPCHAIN_H

#include <vulkan/vulkan.h>

namespace vk
{

class Device;

struct SwapchainInfo
{
    const Device *device = nullptr;
    VkSwapchainKHR swapchain = VK_NULL_HANDLE;
    std::vector<VkImage> images;
    std::vector<VkImageView> imagesViews;
};

class Swapchain
{
private:
    VkSwapchainKHR m_swapchain = VK_NULL_HANDLE;
    const Device *m_device;

    std::vector<VkImage> m_images;
    std::vector<VkImageView> m_imagesViews;
public:
    Swapchain() = default;
    Swapchain(const SwapchainInfo &info);

    void destroy();

    operator VkSwapchainKHR() const;
};

} // namespace vk

#endif // RPG_SWAPCHAIN_H
