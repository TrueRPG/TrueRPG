#ifndef RPG_ILIGHTRENDERSUBSYSTEM_H
#define RPG_ILIGHTRENDERSUBSYSTEM_H

#include "IRenderSubsystem.h"

class IShader;

/**
 * Subsystem of the light rendering system.
 */
class ILightRenderSubsystem
{
public:
    virtual ~ILightRenderSubsystem() = default;

    virtual void draw() = 0;

    virtual void update(float deltaTime) {};

    virtual IShader& getShader() = 0;

    virtual void destroy() {};
};

#endif // RPG_ILIGHTRENDERSUBSYSTEM_H
