#ifndef RPG_SCENE_H
#define RPG_SCENE_H

#include <entt.hpp>
#include "../client/graphics/SpriteBatch.h"

#include "systems/ScriptSystem.h"
#include "systems/RenderSystem.h"
#include "systems/AudioSystem.h"
#include "systems/PhysicsSystem.h"

class Entity;

class Scene
{
    entt::registry m_registry;

    ScriptSystem m_scriptSystem;
    PhysicsSystem m_physicsSystem;
    RenderSystem m_renderSystem;
    AudioSystem m_audioSystem;
public:
    Scene();

    Entity createEntity(const std::string& name = "");

    void destroyEntity(Entity entity);

	void fixedUpdate();

    void update(float deltaTime);

    void destroy();
};

#endif //RPG_SCENE_H
