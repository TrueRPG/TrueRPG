#ifndef RPG_IUIRENDERSUBSYSTEM_H
#define RPG_IUIRENDERSUBSYSTEM_H

class ISpriteBatch;

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
    virtual void draw(ISpriteBatch& batch, glm::vec2 cursor) = 0;

    virtual void update(float deltaTime) {};
};

#endif // RPG_IUIRENDERSUBSYSTEM_H
