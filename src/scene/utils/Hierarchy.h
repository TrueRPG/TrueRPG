#ifndef RPG_HIERARCHY_H
#define RPG_HIERARCHY_H

#include "../Entity.h"

/**
 * Utility class which contains useful functions for the entity hierarchy.
 */
class Hierarchy
{
public:
    /**
     * Add a child to the given entity.
     *
     * @param parent the parent entity
     * @param child the child entity
     */
    static void addChild(Entity parent, Entity child);

    /**
     * Find a child entity by name.
     *
     * @param parent the parent entity
     * @param name the name of the child
     * @return found or empty entity
     */
    static Entity find(Entity parent, std::string name);

    /**
     * Compute transformation for the given entity.
     * We can use this method to find the world position of the child entity.
     *
     * @param entity the entity
     * @return calculated transformation
     */
    static TransformComponent computeTransform(Entity entity);
};

#endif //RPG_HIERARCHY_H
