#include "Engine.h"

#include "window/GlfwWindow.h"

IWindow &Engine::getWindow(int width, int height, const std::string &title)
{
    static GlfwWindow window(width, height, title);
    return window;
}
