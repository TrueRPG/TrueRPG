#ifndef RPG_SRC_CLIENT_GRAPHICS_OPENGL_GLCONTEXT_H
#define RPG_SRC_CLIENT_GRAPHICS_OPENGL_GLCONTEXT_H

#include "../IGraphicsContext.h"

class GLContext : public IGraphicsContext
{
public:

    static GLContext &getInstance();

    void init() override;
    void swapBuffers() override;
};

#endif // RPG_SRC_CLIENT_GRAPHICS_OPENGL_GLCONTEXT_H
