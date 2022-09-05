#ifndef RPG_ENGINE_H
#define RPG_ENGINE_H

#define GLFW_INCLUDE_NONE

#include <string>
#include "window/IWindow.h"
#include "graphics/GraphicsConfig.h"
#include "graphics/IGraphicsContext.h"

class Engine
{
public:
    static IWindow &getWindow(const std::string& title = "");
    static const GraphicsConfig &getGraphicsConfig();
    static IGraphicsContext &getGraphicsContext();
};

#endif // RPG_ENGINE_H
