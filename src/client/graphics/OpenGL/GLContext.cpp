#include "../../../pch.h"
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

IShader &GLContext::createShader(const std::string& vertexPath, const std::string& fragmentPath, ShaderEnabledUniform enabled)
{
    m_shaders.emplace_back(new Shader(Shader::createShader(vertexPath, fragmentPath, enabled)));
    return *m_shaders.back();
}
