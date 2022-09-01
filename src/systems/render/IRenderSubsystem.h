#ifndef RPG_IRENDERSUBSYSTEM_H
#define RPG_IRENDERSUBSYSTEM_H

#include "../../client/graphics/OpenGL/SpriteBatch.h"

/**
 * Subsystem of the rendering system.
 */
class IRenderSubsystem
{
public:
    virtual ~IRenderSubsystem() = default;

    /**
     * Draw using the given sprite batch.
     *
     * @param batch the batch
     */
    virtual void draw(SpriteBatch& batch) = 0;

    virtual void update(float deltaTime) {};

    virtual void destroy() {};
};

#endif // RPG_IRENDERSUBSYSTEM_H
