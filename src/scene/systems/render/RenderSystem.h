#ifndef RPG_RENDERSYSTEM_H
#define RPG_RENDERSYSTEM_H

#include "../../../client/graphics/SpriteBatch.h"
#include "../../components/basic/TransformComponent.h"
#include "IRenderSubsystem.h"
#include "entt.hpp"

class RenderSystem
{
    entt::registry& m_registry;
    Shader m_shader;
    SpriteBatch m_batch;

    std::vector<IRenderSubsystem*> m_subsystems;

public:
    RenderSystem(entt::registry& registry);

    void addSubsystem(IRenderSubsystem& renderSubsystem);

    void draw();

    void destroy();

    static void resize(int width, int height);
};

#endif // RPG_RENDERSYSTEM_H
