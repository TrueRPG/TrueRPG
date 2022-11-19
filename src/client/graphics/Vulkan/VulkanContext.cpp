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

    m_instance = vk::InstanceBuilder()
                     .setApiVersion<VK_API_VERSION_1_2>()
                     .setAppName("TrueRPG")
                     .setEngineName("TrueRPG_Engine")
                     .setExtensions(extensions)
                     .setDebugCallback([](VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
                         VkDebugUtilsMessageTypeFlagsEXT messageTypes,
                         const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
                         void* pUserData) -> VkBool32 {
                             std::clog << pCallbackData->pMessage << std::endl;
                             return VK_FALSE;
                     })
                     .build()
                     .except();
}
