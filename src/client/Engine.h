#ifndef RPG_ENGINE_H
#define RPG_ENGINE_H

#define GLFW_INCLUDE_NONE

#include <string>
#include "window/IWindow.h"

class Engine
{
public:
    static IWindow &getWindow(int width = 0, int height = 0, const std::string& title = "");
};

#endif // RPG_ENGINE_H
