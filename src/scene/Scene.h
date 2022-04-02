#ifndef RPG_SCENE_H
#define RPG_SCENE_H

#include <entt.hpp>
#include "../client/graphics/SpriteBatch.h"

#include "systems/ScriptSystem.h"
#include "systems/render/RenderSystem.h"
#include "systems/AudioSystem.h"
#include "systems/PhysicsSystem.h"
#include "systems/render/TextRenderSystem.h"
#include "systems/render/SpriteRenderSystem.h"
#include "systems/render/WorldMapRenderSystem.h"
#include "systems/render/ui/ButtonRenderSystem.h"
#include "systems/render/ui/UIRenderSystem.h"
#include "systems/render/ui/InventoryRenderSystem.h"

class Entity;

class Scene
{
    entt::registry m_registry;

    ScriptSystem m_scriptSystem;
    PhysicsSystem m_physicsSystem;

    ButtonRenderSystem m_buttonSystem;
    InventoryRenderSystem m_inventorySystem;

    WorldMapRenderSystem m_worldMapRenderSystem;
    SpriteRenderSystem m_spriteRenderSystem;
    UIRenderSystem m_uiRenderSystem;
    TextRenderSystem m_textRenderSystem;

    RenderSystem m_renderSystem;

    AudioSystem m_audioSystem;
public:
    Scene();

    Entity createEntity(const std::string& name = "");

    void destroyEntity(Entity entity);

    void update(float deltaTime);

    void destroy();
};

#endif //RPG_SCENE_H
