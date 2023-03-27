#include "../../../pch.h"
#include "VulkanContext.h"
#include "builders/InstanceBuilder.h"
#include "builders/DeviceBuilder.h"
#include "builders/PipelineBuilder.h"
#include "builders/AllocatorBuilder.h"
#include "../../Engine.h"

void VulkanContext::init()
{
    initInstance();
    initDevice();
    initSwapchain();
    initPipelineLayout();
    initRenderPass();

    initAllocator();
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
    std::vector<const char *> extensions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };
    if constexpr (DEBUG_ENABLED)
    {
        layers.emplace_back("VK_LAYER_KHRONOS_validation");
    }

    m_device = vk::DeviceBuilder(m_instance, m_surface)
                   .setExtensions(extensions)
                   .setLayers(layers)
                   .build()
                   .except();
}

void VulkanContext::initSwapchain()
{
    m_swapchain = m_device.createSwapchain(m_surface).except();
}

void VulkanContext::initAllocator()
{
    m_allocator = vk::AllocatorBuilder()
                      .setApiVersion(VK_API_VERSION_1_2)
                      .setDevice(m_device)
                      .setInstance(m_instance)
                      .build();
}


void VulkanContext::destroy()
{
    m_allocator.destroy();

    vkDestroyRenderPass(m_device, m_renderPass, nullptr);
    vkDestroyDescriptorSetLayout(m_device, m_descriptorSetLayout, nullptr);
    vkDestroyPipelineLayout(m_device, m_pipelineLayout, nullptr);
    m_swapchain.destroy();
    m_device.destroy();
    vkDestroySurfaceKHR(m_instance, m_surface.surface, nullptr);
    m_instance.destroy();
}

VulkanContext &VulkanContext::getInstance()
{
    static VulkanContext instance;
    return instance;
}

void VulkanContext::initPipelineLayout()
{
    std::array<VkDescriptorSetLayoutBinding, 2> layoutBindings{};
    layoutBindings[0].binding = 0;
    layoutBindings[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    layoutBindings[0].descriptorCount = 1;
    layoutBindings[0].stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
    layoutBindings[0].pImmutableSamplers = nullptr;

    layoutBindings[1].binding = 1;
    layoutBindings[1].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    layoutBindings[1].descriptorCount = 16;
    layoutBindings[1].stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
    layoutBindings[1].pImmutableSamplers = nullptr;

    VkDescriptorSetLayoutCreateInfo layoutCreateInfo{};
    layoutCreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    layoutCreateInfo.bindingCount = layoutBindings.size();
    layoutCreateInfo.pBindings = layoutBindings.data();

    if (vkCreateDescriptorSetLayout(m_device, &layoutCreateInfo, nullptr, &m_descriptorSetLayout) != VK_SUCCESS)
    {
        throw;
    }

    VkPipelineLayoutCreateInfo info{};
    info.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    info.setLayoutCount = 1;
    info.pSetLayouts = &m_descriptorSetLayout;
    info.pushConstantRangeCount = 0;
    info.pPushConstantRanges = nullptr;

    VkResult result = vkCreatePipelineLayout(m_device, &info, nullptr, &m_pipelineLayout);
    if (result !=  VK_SUCCESS)
        throw;
}

void VulkanContext::initRenderPass()
{
    std::array<VkAttachmentDescription, 2> colorAttachments{};
    colorAttachments[0].format = m_swapchain.getFormat();
    colorAttachments[0].samples = VK_SAMPLE_COUNT_1_BIT;
    colorAttachments[0].loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    colorAttachments[0].storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    colorAttachments[0].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    colorAttachments[0].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    colorAttachments[0].initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    colorAttachments[0].finalLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

    colorAttachments[1].format = m_swapchain.getFormat();
    colorAttachments[1].samples = VK_SAMPLE_COUNT_1_BIT;
    colorAttachments[1].loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    colorAttachments[1].storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    colorAttachments[1].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    colorAttachments[1].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    colorAttachments[1].initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    colorAttachments[1].finalLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

    std::array<VkAttachmentReference, 2> colorReferences{};
    colorReferences[0].attachment = 0;
    colorReferences[0].layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
    colorReferences[1].attachment = 1;
    colorReferences[1].layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    VkSubpassDescription subpass{};
    subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpass.colorAttachmentCount = colorReferences.size();
    subpass.pColorAttachments = colorReferences.data();

    VkRenderPassCreateInfo info{};
    info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    info.attachmentCount = colorAttachments.size();
    info.pAttachments = colorAttachments.data();
    info.subpassCount = 1;
    info.pSubpasses = &subpass;

    VkResult result = vkCreateRenderPass(m_device, &info, nullptr, &m_renderPass);
    if (result != VK_SUCCESS)
        throw;
}
