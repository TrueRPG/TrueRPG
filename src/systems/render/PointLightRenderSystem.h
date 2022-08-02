#ifndef RPG_POINTLIGHTRENDERSYSTEM_H
#define RPG_POINTLIGHTRENDERSYSTEM_H

#include "IRenderSubsystem.h"
#include "entt.hpp"
#include "ILightRenderSubsystem.h"

class PointLightRenderSystem : public ILightRenderSubsystem
{
    entt::registry& m_registry;
    Shader m_shader;

public:
    PointLightRenderSystem(entt::registry& registry);

    void draw() override;

    Shader& getShader() override;
};

#endif // RPG_POINTLIGHTRENDERSYSTEM_H
