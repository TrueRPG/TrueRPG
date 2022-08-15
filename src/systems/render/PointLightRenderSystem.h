#ifndef RPG_POINTLIGHTRENDERSYSTEM_H
#define RPG_POINTLIGHTRENDERSYSTEM_H

#include "IRenderSubsystem.h"
#include "entt.hpp"
#include "ILightRenderSubsystem.h"

#include "../../client/graphics/Quad.h"

class PointLightRenderSystem : public ILightRenderSubsystem
{
    entt::registry& m_registry;
    Shader m_shader;
    Quad m_quad;
public:
    explicit PointLightRenderSystem(entt::registry& registry);
    ~PointLightRenderSystem() override;

    void draw() override;

    Shader& getShader() override;
};

#endif // RPG_POINTLIGHTRENDERSYSTEM_H
