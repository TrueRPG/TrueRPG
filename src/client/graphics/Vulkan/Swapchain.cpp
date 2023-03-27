#include "../../../pch.h"
#include "Swapchain.h"
#include "Device.h"

namespace vk
{

Swapchain::Swapchain(const SwapchainInfo &info)
    : m_device(info.device),
      m_swapchain(info.swapchain),
      m_surfaceFormat(info.surfaceFormat),
      m_presentMode(info.presentMode),
      m_extent(info.extent),
      m_images(info.images),
      m_imagesViews(info.imagesViews)
{}

VkFormat Swapchain::getFormat() const
{
    return m_surfaceFormat.format;
}

void Swapchain::destroy()
{
    for (const auto &imageView : m_imagesViews)
        vkDestroyImageView(*m_device, imageView, nullptr);

    vkDestroySwapchainKHR(*m_device, m_swapchain, nullptr);
}

Swapchain::operator VkSwapchainKHR() const
{
    return m_swapchain;
}

} // namespace vk