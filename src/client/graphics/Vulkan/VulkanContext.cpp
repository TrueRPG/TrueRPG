#include "../../../pch.h"
#include "VulkanContext.h"
#include "builders/InstanceBuilder.h"
#include "builders/DeviceBuilder.h"
#include "../../Engine.h"

void VulkanContext::init()
{
    initInstance();
    initDevice();
}

void VulkanContext::initInstance()
{
    IWindow &wnd = Engine::getWindow();
    std::vector<const char *> extensions = wnd.getRequiredExtensions();
    std::vector<const char *> layers;

    if constexpr (DEBUG_ENABLED)
    {
        extensions.emplace_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
        layers.emplace_back("VK_LAYER_KHRONOS_validation");
    }

    m_instance = vk::InstanceBuilder()
                     .setApiVersion<VK_API_VERSION_1_2>()
                     .setAppName("TrueRPG")
                     .setEngineName("TrueRPG_Engine")
                     .setExtensions(extensions)
                     .setLayers(layers)
                     .setDebugCallback([](VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
                         VkDebugUtilsMessageTypeFlagsEXT messageTypes,
                         const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
                         void* pUserData) -> VkBool32 {
                             logger::debug(pCallbackData->pMessage);
                             return VK_FALSE;
                     })
                     .setDesiredDeviceType(VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
                     .build()
                     .except();

    m_surface = m_instance.createSurface().except();
}

void VulkanContext::initDevice()
{
    std::vector<const char *> layers;
    if constexpr (DEBUG_ENABLED)
    {
        layers.emplace_back("VK_LAYER_KHRONOS_validation");
    }

    m_device = vk::DeviceBuilder(m_instance, m_surface)
                   .setLayers(layers)
                   .build()
                   .except();
}

void VulkanContext::destroy()
{
    m_device.destroy();
    vkDestroySurfaceKHR(m_instance, m_surface.surface, nullptr);
    m_instance.destroy();
}

VulkanContext &VulkanContext::getInstance()
{
    static VulkanContext instance;
    return instance;
}
