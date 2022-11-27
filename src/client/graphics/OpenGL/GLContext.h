#ifndef RPG_SRC_CLIENT_GRAPHICS_OPENGL_GLCONTEXT_H
#define RPG_SRC_CLIENT_GRAPHICS_OPENGL_GLCONTEXT_H

#include "../IGraphicsContext.h"
#include "Shader.h"
#include "../ITexture.h"

class GLContext : public IGraphicsContext
{
private:
    std::vector<std::shared_ptr<IShader>> m_shaders;
    std::vector<std::shared_ptr<ITexture>> m_textures;
public:

    static GLContext &getInstance();

    void init() override;
    void swapBuffers() override;

    IShader &createShader(const std::string& vertexPath, const std::string& fragmentPath, ShaderEnabledUniform enabled = {}) override;

    ITexture &createTexture(const std::string &path) override;
    ITexture &createTexture(unsigned int id, const std::string &path, int width, int height) override;
    ITexture &createEmptyTexture() override;

    void destroy() override { }
private:
    GLContext() = default;
};

#endif // RPG_SRC_CLIENT_GRAPHICS_OPENGL_GLCONTEXT_H
