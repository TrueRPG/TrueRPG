#include "VulkanContext.h"
#include "builders/InstanceBuilder.h"
#include "../../Engine.h"

void VulkanContext::init()
{
    initInstance();
}

void VulkanContext::initInstance()
{
    IWindow &wnd = Engine::getWindow();
    std::vector<const char *> extensions = wnd.getRequiredExtensions();
    std::vector<const char *> layers;

    if constexpr (vk::DEBUG_ENABLED)
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
}
