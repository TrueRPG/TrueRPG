#include "Scene.h"

#include "Entity.h"
#include "components/basic/HierarchyComponent.h"
#include "components/basic/NameComponent.h"
#include "systems/render/ui/ButtonRenderSystem.h"

// TODO: create ISystem interface
Scene::Scene()
    : m_scriptSystem(m_registry),
      m_physicsSystem(m_registry),
      m_buttonSystem(m_registry, Texture::createEmpty()),
      m_inventorySystem(m_registry, Texture::createEmpty()),
      m_worldMapRenderSystem(m_registry),
      m_spriteRenderSystem(m_registry),
      m_uiRenderSystem(m_registry),
      m_textRenderSystem(m_registry),
      m_renderSystem(m_registry),
      m_audioSystem(m_registry)
{
    m_uiRenderSystem.addSubsystem(m_buttonSystem);
    m_uiRenderSystem.addSubsystem(m_inventorySystem);

    m_renderSystem.addSubsystem(m_worldMapRenderSystem);
    m_renderSystem.addSubsystem(m_spriteRenderSystem);
    m_renderSystem.addSubsystem(m_uiRenderSystem);
    m_renderSystem.addSubsystem(m_textRenderSystem);
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
    m_scriptSystem.destroyScript(entity.m_entity);
    m_registry.destroy(entity.m_entity);
}

void Scene::update(float deltaTime)
{
    m_scriptSystem.update(deltaTime);
    m_physicsSystem.update(deltaTime);
    m_renderSystem.draw();
    m_audioSystem.update();
}

void Scene::destroy()
{
    m_scriptSystem.destroy();
    m_renderSystem.destroy();
    m_audioSystem.destroy();
}
