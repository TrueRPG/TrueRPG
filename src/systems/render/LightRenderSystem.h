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
    Shader m_shader;
    i32 m_time;
public:
    explicit LightRenderSystem(entt::registry &registry);

    void draw(SpriteBatch &batch) override;
    void fixedUpdate() override;

    void setShader(Shader shader);

private:
    void drawGlobalLight(Light &light);
    void drawLightSources(Light &light, SpriteBatch &batch);
};

#endif // RPG_LIGHTRENDERSYSTEM_H
