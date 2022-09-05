#include "GLContext.h"
#include "../../Engine.h"

GLContext &GLContext::getInstance()
{
    static GLContext instance{};
    return instance;
}

void GLContext::init()
{
    auto &window = Engine::getWindow();
    window.makeContextCurrent();
    window.loadGL();
    window.swapInterval(0);
}

void GLContext::swapBuffers()
{
    Engine::getWindow().swapBuffers();
}
