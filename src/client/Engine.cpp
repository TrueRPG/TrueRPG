#include "../pch.h"
#include "Engine.h"

#include "window/GlfwWindow.h"
#include "graphics/OpenGL/GLContext.h"
#include "graphics/Vulkan/VulkanContext.h"

IWindow &Engine::getWindow(const std::string &title)
{
    static GlfwWindow window(getGraphicsConfig().getWidth(), getGraphicsConfig().getHeight(), title);
    return window;
}

const GraphicsConfig &Engine::getGraphicsConfig()
{
    static GraphicsConfig config(TRUERPG_RES_DIR "/configs/graphics.yml");
    return config;
}

IGraphicsContext &Engine::getGraphicsContext()
{
    static GraphicsAPI api = getGraphicsConfig().getApi();
    switch (api)
    {
    case GraphicsAPI::OpenGL:
        return GLContext::getInstance();
    case GraphicsAPI::Vulkan:
        return VulkanContext::getInstance();
    default:
        throw;
    }
}
