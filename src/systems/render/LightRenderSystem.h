#ifndef RPG_LIGHTRENDERSYSTEM_H
#define RPG_LIGHTRENDERSYSTEM_H

#include "IRenderSubsystem.h"
#include <entt.hpp>
#include "../../client/graphics/Shader.h"
#include "../../utils/Types.h"

class Light;

class LightRenderSystem : public IRenderSubsystem
{
private:
    entt::registry &m_registry;
    // TODO: time filed can be definitely in world/game state for cycle
    i32 m_time;
public:
    explicit LightRenderSystem(entt::registry &registry);

    void draw(SpriteBatch &batch) override;
    void fixedUpdate() override;
private:
    void drawGlobalLight(Light &light, SpriteBatch &batch);
    void drawLightSources(Light &light, SpriteBatch &batch);
};

#endif // RPG_LIGHTRENDERSYSTEM_H
