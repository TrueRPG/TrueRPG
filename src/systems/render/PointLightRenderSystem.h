#ifndef RPG_POINTLIGHTRENDERSYSTEM_H
#define RPG_POINTLIGHTRENDERSYSTEM_H

#include "IRenderSubsystem.h"
#include "entt.hpp"
#include "ILightRenderSubsystem.h"

#include "../../client/graphics/VertexArray.h"
#include "../../client/graphics/Buffer.h"

class PointLightRenderSystem : public ILightRenderSubsystem
{
    entt::registry& m_registry;
    Shader m_shader;
    VertexArray m_quadVao;
    Buffer m_quadVbo{GL_ARRAY_BUFFER};
public:
    PointLightRenderSystem(entt::registry& registry);
    ~PointLightRenderSystem() override;

    void draw() override;

    Shader& getShader() override;
};

#endif // RPG_POINTLIGHTRENDERSYSTEM_H
