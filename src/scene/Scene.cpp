#include "Scene.h"

#include "Entity.h"
#include "../components/basic/HierarchyComponent.h"
#include "../components/basic/NameComponent.h"
#include "../components/basic/TransformComponent.h"

Scene::~Scene()
{
    for (const auto &system : m_systems)
    {
        delete system;
    }
}

Entity Scene::createEntity(const std::string &name)
{
    Entity entity(m_registry.create(), &m_registry);
    // Add the mandatory components, that each entity must have
    entity.addComponent<TransformComponent>();
    entity.addComponent<HierarchyComponent>();
    auto &nameComponent = entity.addComponent<NameComponent>();
    nameComponent.name = name.empty() ? "Entity" : name;
    return entity;
}

void Scene::destroyEntity(Entity entity)
{
    m_registry.destroy(entity.m_entity);
}

void Scene::create()
{
    for (const auto &system : m_systems)
    {
        system->create();
    }
}

void Scene::update(float deltaTime)
{
    for (const auto &system : m_systems)
    {
        system->update(deltaTime);
    }
}

void Scene::destroy()
{
    for (const auto &system : m_systems)
    {
        system->destroy();
    }
}
