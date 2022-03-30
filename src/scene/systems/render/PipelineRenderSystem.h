#ifndef RPG_PIPELINERENDERSYSTEM_H
#define RPG_PIPELINERENDERSYSTEM_H

#include "entt.hpp"
#include "../../../client/graphics/SpriteBatch.h"
#include "../../components/basic/TransformComponent.h"
#include "UIRenderSystem.h"
#include "IRenderSystem.h"

class PipelineRenderSystem
{
    entt::registry& m_registry;
    Shader m_shader;
    SpriteBatch m_batch;

    std::vector<IRenderSystem*> m_pipeline;

public:
    PipelineRenderSystem(entt::registry& registry, std::vector<IRenderSystem*> pipeline);

    void draw();

    void destroy();

    static void resize(int width, int height);
};

#endif // RPG_PIPELINERENDERSYSTEM_H
