#ifndef RPG_SRC_CLIENT_GRAPHICS_OPENGL_GLCONTEXT_H
#define RPG_SRC_CLIENT_GRAPHICS_OPENGL_GLCONTEXT_H

#include "../IGraphicsContext.h"
#include "Shader.h"

class GLContext : public IGraphicsContext
{
private:
    std::vector<std::shared_ptr<IShader>> m_shaders;
    i32 m_shaderIndex = 0;
public:

    static GLContext &getInstance();

    void init() override;
    void swapBuffers() override;

    IShader &createShader(const std::string& vertexPath, const std::string& fragmentPath, ShaderEnabledUniform enabled = {}) override;

    void destroy() override { }
private:
    GLContext() = default;
};

#endif // RPG_SRC_CLIENT_GRAPHICS_OPENGL_GLCONTEXT_H
