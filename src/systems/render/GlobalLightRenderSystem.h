#ifndef RPG_GLOBALLIGHTRENDERSYSTEM_H
#define RPG_GLOBALLIGHTRENDERSYSTEM_H

#include "ILightRenderSubsystem.h"
#include "entt.hpp"
#include "../../client/graphics/Quad.h"

class GlobalLightRenderSystem : public ILightRenderSubsystem
{
    entt::registry& m_registry;
    Shader m_shader;
    Quad m_quad;
public:
    explicit GlobalLightRenderSystem(entt::registry& registry);

    void draw() override;

    Shader& getShader() override;

    void destroy() override;
};

#endif // RPG_GLOBALLIGHTRENDERSYSTEM_H
