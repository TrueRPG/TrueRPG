#ifndef RPG_SRC_CLIENT_GRAPHICS_IGRAPHICSCONTEXT_H
#define RPG_SRC_CLIENT_GRAPHICS_IGRAPHICSCONTEXT_H

#include "IShader.h"
#include "ITexture.h"
#include "ShaderInfo.h"
#include "ISpriteBatch.h"

class IGraphicsContext
{
public:
    virtual ~IGraphicsContext() = default;

    virtual void init() = 0;
    virtual void swapBuffers() = 0;
    virtual ISpriteBatch &getSpriteBatch() = 0;
    virtual ISpriteBatch &getSpriteBatch(IShader *shader, int spriteCount = 2000) = 0;
    virtual IShader &createShader(const std::string& vertexPath, const std::string& fragmentPath, ShaderEnabledUniform enabled = {}) = 0;
    virtual ITexture &createTexture(const std::string &path) = 0;
    virtual ITexture &createTexture(unsigned int id, const std::string& path, int width, int height) = 0;
    virtual ITexture &createEmptyTexture() = 0;
    virtual void destroy() = 0;
};

#endif // RPG_SRC_CLIENT_GRAPHICS_IGRAPHICSCONTEXT_H
