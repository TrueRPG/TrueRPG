#ifndef RPG_SRC_CLIENT_GRAPHICS_IGRAPHICSCONTEXT_H
#define RPG_SRC_CLIENT_GRAPHICS_IGRAPHICSCONTEXT_H

#include "IShader.h"
#include "ShaderInfo.h"

class IGraphicsContext
{
public:
    virtual ~IGraphicsContext() = default;

    virtual void init() = 0;
    virtual void swapBuffers() = 0;
    virtual IShader &createShader(const std::string& vertexPath, const std::string& fragmentPath, ShaderEnabledUniform enabled = {}) = 0;
    virtual void destroy() = 0;
};

#endif // RPG_SRC_CLIENT_GRAPHICS_IGRAPHICSCONTEXT_H
