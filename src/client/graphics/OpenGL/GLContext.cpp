#include "../../../pch.h"
#include "GLContext.h"
#include "../../Engine.h"
#include "SpriteBatch.h"
#include "Shader.h"
#include "Texture.h"

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
ITexture &GLContext::createTexture(const std::string &path)
{
    m_textures.emplace_back(new Texture(Texture::create(path)));
    return *m_textures.back();
}
ITexture &GLContext::createEmptyTexture()
{
    static Texture empty = Texture::createEmpty();
    return empty;
}

ITexture &GLContext::createTexture(unsigned int id, const std::string &path, int width, int height)
{
    m_textures.emplace_back(new Texture(id, path, width, height));
    return *m_textures.back();
}

ISpriteBatch &GLContext::createSpriteBatch(IShader *shader, int spriteCount)
{
    m_batches.emplace_back(new SpriteBatch(shader, spriteCount));
    return *m_batches.back();
}
