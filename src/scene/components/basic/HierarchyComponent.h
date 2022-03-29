#ifndef RPG_HIERARCHYCOMPONENT_H
#define RPG_HIERARCHYCOMPONENT_H

#include "../../Entity.h"

struct HierarchyComponent
{
    std::size_t children{}; // The number of the children of the entity
    Entity firstChild{}; // The first child of the entity
    Entity prev{}; // The previous child of the parent
    Entity next{}; // The next child of the parent
    Entity parent{}; // The parent entity
};

#endif //RPG_HIERARCHYCOMPONENT_H
