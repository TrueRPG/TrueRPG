#ifndef RPG_IUIRENDERSUBSYSTEM_H
#define RPG_IUIRENDERSUBSYSTEM_H

#include "../../../client/graphics/SpriteBatch.h"

/**
 * Subsystem of the UI rendering system.
 */
class IUIRenderSubsystem
{
public:
    virtual ~IUIRenderSubsystem() = default;

    /**
     * Draw UI using the given sprite batch.
     *
     * @param batch the batch
     * @param cursor current cursor position
     */
    virtual void draw(SpriteBatch& batch, glm::vec2 cursor) = 0;
};

#endif // RPG_IUIRENDERSUBSYSTEM_H
