#ifndef RPG_VULKANCONTEXT_H
#define RPG_VULKANCONTEXT_H

#include "../IGraphicsContext.h"
#include "Instance.h"

class VulkanContext : IGraphicsContext
{
private:
    vk::Instance m_instance;
public:
    void init() override;

    void swapBuffers() override
    {

    }

    IShader &createShader(const std::string &vertexPath, const std::string &fragmentPath, ShaderEnabledUniform enabled = {}) override
    {
        
    }

    void destroy() override;
private:
    void initInstance();
};

#endif