#include "../../../pch.h"
#include "Device.h"
#include "Surface.h"
#include "../../Engine.h"

namespace vk
{
Device::Device(VkDevice device, VkPhysicalDevice physicalDevice, Queue graphics, Queue presentation)
    : m_device(device),
      m_physicalDevice(physicalDevice),
      m_graphics(graphics),
      m_presentation(presentation)
{}

VkPhysicalDevice Device::getPhysicalDevice() const
{
    return m_physicalDevice;
}

Queue Device::getGraphicsQueue() const
{
    return m_graphics;
}

Queue Device::getPresentationQueue() const
{
    return m_presentation;
}

static VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR capabilities)
{
    if (capabilities.currentExtent.width != std::numeric_limits<u32>::max())
    {
        return capabilities.currentExtent;
    }

    return capabilities.currentExtent;
}

ObjResult<Swapchain> Device::createSwapchain(const Surface &surface) const
{
    SwapchainInfo swapchainInfo;
    auto capabilities = surface.getCapabilities();
    auto presentModes = surface.getPresentModes();
    auto formats = surface.getFormats();

    VkSurfaceFormatKHR choosenFormat = formats[0];
    VkPresentModeKHR choosenPresentMode = presentModes[0];
    VkExtent2D choosenExtent = chooseSwapExtent(capabilities);
    u32 imageCount = capabilities.minImageCount + 1;

    for (const auto &format : formats)
    {
        if (format.format == VK_FORMAT_B8G8R8A8_SRGB && format.colorSpace == VK_COLORSPACE_SRGB_NONLINEAR_KHR)
        {
            choosenFormat = format;
            break;
        }
    }

    for (const auto &presentMode : presentModes)
    {
        if (presentMode == VK_PRESENT_MODE_MAILBOX_KHR)
        {
            choosenPresentMode = presentMode;
            break;
        }
    }

    if (capabilities.maxImageCount > 0 && imageCount > capabilities.maxImageCount)
        imageCount = capabilities.maxImageCount;

    VkSwapchainCreateInfoKHR info{};
    info.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    info.surface = surface.surface;
    info.minImageCount = imageCount;
    info.imageFormat = choosenFormat.format;
    info.imageColorSpace = choosenFormat.colorSpace;
    info.imageExtent = choosenExtent;
    info.imageArrayLayers = 1;
    info.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
    info.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;

    std::set<u32> familyIndices = {m_graphics.familyIndex.value(), m_presentation.familyIndex.value()};
    std::vector<u32> indices{familyIndices.begin(), familyIndices.end()};

    if (familyIndices.size() > 1)
    {
        info.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
        info.queueFamilyIndexCount = familyIndices.size();
        info.pQueueFamilyIndices = indices.data();
    }

    info.preTransform = capabilities.currentTransform;
    info.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    info.presentMode = choosenPresentMode;
    info.clipped = VK_TRUE;
    info.oldSwapchain = VK_NULL_HANDLE;

    VkResult res = vkCreateSwapchainKHR(m_device, &info, nullptr, &swapchainInfo.swapchain);
    if (res != VK_SUCCESS)
    {

    }

    logger::debug("swapchain has been created");

    swapchainInfo.device = this;

    vkGetSwapchainImagesKHR(m_device, swapchainInfo.swapchain, &imageCount, nullptr);
    swapchainInfo.images.resize(imageCount);
    vkGetSwapchainImagesKHR(m_device, swapchainInfo.swapchain, &imageCount, swapchainInfo.images.data());

    for (const auto &image : swapchainInfo.images)
    {
        VkImageView imageView;
        VkImageViewCreateInfo imageViewCreateInfo{};
        imageViewCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        imageViewCreateInfo.image = image;
        imageViewCreateInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
        imageViewCreateInfo.format = choosenFormat.format;
        imageViewCreateInfo.components = {
            VK_COMPONENT_SWIZZLE_IDENTITY,
            VK_COMPONENT_SWIZZLE_IDENTITY,
            VK_COMPONENT_SWIZZLE_IDENTITY,
            VK_COMPONENT_SWIZZLE_IDENTITY
        };
        imageViewCreateInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        imageViewCreateInfo.subresourceRange.baseMipLevel = 0;
        imageViewCreateInfo.subresourceRange.levelCount = 1;
        imageViewCreateInfo.subresourceRange.baseArrayLayer = 0;
        imageViewCreateInfo.subresourceRange.layerCount = 1;

        res = vkCreateImageView(m_device, &imageViewCreateInfo, nullptr, &imageView);
        if (res != VK_SUCCESS)
        {}

        swapchainInfo.imagesViews.emplace_back(imageView);
    }

    swapchainInfo.surfaceFormat = choosenFormat;
    swapchainInfo.presentMode = choosenPresentMode;
    swapchainInfo.extent = choosenExtent;
    
    return ObjResult<Swapchain>(swapchainInfo);
}

void Device::destroy()
{
    vkDestroyDevice(m_device, nullptr);
}

Device::operator VkDevice() const
{
    return m_device;
}

} // namespace vk