#ifndef RPG_VULKANCONTEXT_H
#define RPG_VULKANCONTEXT_H

#include "../IGraphicsContext.h"
#include "Instance.h"
#include "Allocator.h"
#include "../ITexture.h"

class VulkanContext : public IGraphicsContext
{
private:
    vk::Instance m_instance;
    vk::Surface m_surface;
    vk::Device m_device;
    vk::Allocator m_allocator;
    vk::Swapchain m_swapchain;
    VkDescriptorSetLayout m_descriptorSetLayout;
    VkPipelineLayout m_pipelineLayout;
    VkRenderPass m_renderPass;
public:
    static VulkanContext &getInstance();

    void init() override;

    void swapBuffers() override
    {

    }

    ISpriteBatch &createSpriteBatch(IShader *shader, int spriteCount = 2000) override
    {
        
    }

    IShader &createShader(const std::string &vertexPath, const std::string &fragmentPath, ShaderEnabledUniform enabled = {}) override
    {
        
    }

    ITexture &createTexture(const std::string &path) override
    {

    }

    ITexture &createTexture(unsigned int id, const std::string &path, int width, int height) override
    {

    }

    ITexture &createEmptyTexture() override
    {

    }

    void destroy() override;
private:
    VulkanContext() = default;

    void initInstance();
    void initDevice();
    void initSwapchain();
    void initPipelineLayout();
    void initRenderPass();

    void initAllocator();
};

#endif