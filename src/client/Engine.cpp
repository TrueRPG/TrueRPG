#include "../pch.h"
#include "Engine.h"

#include "window/GlfwWindow.h"
#include "graphics/GraphicsConfig.h"

IWindow &Engine::getWindow(const std::string &title)
{
    static GlfwWindow window(getGraphicsConfig().getWidth(), getGraphicsConfig().getHeight(), title);
    return window;
}

GraphicsConfig &Engine::getGraphicsConfig()
{
    static GraphicsConfig config(TRUERPG_RES_DIR "/configs/graphics.yml");
    return config;
}
