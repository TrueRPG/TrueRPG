#ifndef RPG_SCENE_H
#define RPG_SCENE_H

#include <entt.hpp>
#include "../client/graphics/SpriteBatch.h"

#include "systems/ScriptSystem.h"
#include "systems/render/PipelineRenderSystem.h"
#include "systems/AudioSystem.h"
#include "systems/PhysicsSystem.h"
#include "systems/render/TextRenderSystem.h"
#include "systems/render/SpriteRenderSystem.h"
#include "systems/render/WorldMapRenderSystem.h"

class Entity;

class Scene
{
    entt::registry m_registry;

    ScriptSystem m_scriptSystem;
    PhysicsSystem m_physicsSystem;

    WorldMapRenderSystem m_worldMapRenderSystem;
    SpriteRenderSystem m_spriteRenderSystem;
    UIRenderSystem m_uiRenderSystem;
    TextRenderSystem m_textRenderSystem;

    PipelineRenderSystem m_renderSystem;

    AudioSystem m_audioSystem;
public:
    Scene();

    Entity createEntity(const std::string& name = "");

    void destroyEntity(Entity entity);

    void update(float deltaTime);

    void destroy();
};

#endif //RPG_SCENE_H
