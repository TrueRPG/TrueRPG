#ifndef RPG_ISYSTEM_H
#define RPG_ISYSTEM_H

/**
 * System interface.
 */
class ISystem
{
public:
    virtual ~ISystem() = default;

    /**
     * Create the system.
     */
    virtual void create() {};

    virtual void fixedUpdate() {};

    /**
     * Update the system.
     */
    virtual void update(float deltaTime) {};

    /**
     * Destroy the system.
     */
    virtual void destroy() {};
};


#endif // RPG_ISYSTEM_H
