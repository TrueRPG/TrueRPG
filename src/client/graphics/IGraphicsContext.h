#ifndef RPG_SRC_CLIENT_GRAPHICS_IGRAPHICSCONTEXT_H
#define RPG_SRC_CLIENT_GRAPHICS_IGRAPHICSCONTEXT_H

class IGraphicsContext
{
public:
    virtual ~IGraphicsContext() = default;

    virtual void init() = 0;
    virtual void swapBuffers() = 0;
};

#endif // RPG_SRC_CLIENT_GRAPHICS_IGRAPHICSCONTEXT_H
