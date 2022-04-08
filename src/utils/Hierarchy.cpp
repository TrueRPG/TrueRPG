#include "Hierarchy.h"

#include "../components/basic/HierarchyComponent.h"
#include "../components/basic/NameComponent.h"

void Hierarchy::addChild(Entity parent, Entity child)
{
    auto &parentHierarchy = parent.getComponent<HierarchyComponent>();
    auto &childHierarchy = child.getComponent<HierarchyComponent>();

    Entity firstChild = parentHierarchy.firstChild;
    if (firstChild)
    {
        auto &firstParentChild = firstChild.getComponent<HierarchyComponent>();
        firstParentChild.prev = child;
    }
    childHierarchy.next = firstChild;
    parentHierarchy.firstChild = child;
    parentHierarchy.children++;

    childHierarchy.parent = parent;
}

Entity Hierarchy::find(Entity parent, const std::string& name)
{
    auto &parentHierarchy = parent.getComponent<HierarchyComponent>();
    auto current = parentHierarchy.firstChild;

    while (current)
    {
        if (current.getComponent<NameComponent>().name == name)
        {
            return current;
        }
        Entity entity = find(current, name);
        if (entity) return entity;
        current = current.getComponent<HierarchyComponent>().next;
    }
    return {};
}

TransformComponent Hierarchy::computeTransform(Entity entity)
{
    auto entityTransform = entity.getComponent<TransformComponent>();

    auto &entityHierarchy = entity.getComponent<HierarchyComponent>();
    auto current = entityHierarchy.parent;

    while (current)
    {
        auto currentTransform = current.getComponent<TransformComponent>();
        entityTransform.position += currentTransform.position;
        entityTransform.scale *= currentTransform.scale;

        current = current.getComponent<HierarchyComponent>().parent;
    }

    return entityTransform;
}
