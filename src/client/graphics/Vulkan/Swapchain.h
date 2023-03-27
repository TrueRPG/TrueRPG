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
    VkSurfaceFormatKHR surfaceFormat;
    VkPresentModeKHR presentMode;
    VkExtent2D extent;
};

class Swapchain
{
private:
    VkSwapchainKHR m_swapchain = VK_NULL_HANDLE;
    const Device *m_device = nullptr;

    VkSurfaceFormatKHR m_surfaceFormat{};
    VkPresentModeKHR m_presentMode{};
    VkExtent2D m_extent{};

    std::vector<VkImage> m_images;
    std::vector<VkImageView> m_imagesViews;
public:
    Swapchain() = default;
    explicit Swapchain(const SwapchainInfo &info);

    [[nodiscard]] VkFormat getFormat() const;

    void destroy();

    operator VkSwapchainKHR() const;
};

} // namespace vk

#endif // RPG_SWAPCHAIN_H
