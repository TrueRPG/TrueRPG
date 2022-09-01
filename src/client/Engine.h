#ifndef RPG_ENGINE_H
#define RPG_ENGINE_H

#define GLFW_INCLUDE_NONE

#include <string>
#include "window/IWindow.h"

class GraphicsConfig;

class Engine
{
public:
    static IWindow &getWindow(const std::string& title = "");
    static GraphicsConfig &getGraphicsConfig();
};

#endif // RPG_ENGINE_H
