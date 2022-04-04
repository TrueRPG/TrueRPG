#ifndef RPG_RENDERSYSTEM_H
#define RPG_RENDERSYSTEM_H

#include <entt.hpp>
#include "../../client/graphics/SpriteBatch.h"
#include "../components/basic/TransformComponent.h"

class RenderSystem
{
    entt::registry& m_registry;
    Shader m_shader;
    SpriteBatch m_batch;
public:
    RenderSystem(entt::registry& registry);

    void draw(float deltaTime);

    void destroy();

    static void resize(int width, int height);
};

#endif //RPG_RENDERSYSTEM_H
