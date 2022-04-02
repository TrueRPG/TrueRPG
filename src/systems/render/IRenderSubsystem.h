#ifndef RPG_IRENDERSUBSYSTEM_H
#define RPG_IRENDERSUBSYSTEM_H

#include "../../client/graphics/SpriteBatch.h"

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
};

#endif // RPG_IRENDERSUBSYSTEM_H
