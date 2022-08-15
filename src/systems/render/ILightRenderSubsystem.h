#ifndef RPG_ILIGHTRENDERSUBSYSTEM_H
#define RPG_ILIGHTRENDERSUBSYSTEM_H

#include "../../client/graphics/SpriteBatch.h"
#include "IRenderSubsystem.h"

/**
 * Subsystem of the light rendering system.
 */
class ILightRenderSubsystem
{
public:
    virtual ~ILightRenderSubsystem() = default;

    virtual void draw() = 0;

    virtual void update(float deltaTime) {};

    virtual Shader& getShader() = 0;
};

#endif // RPG_ILIGHTRENDERSUBSYSTEM_H
