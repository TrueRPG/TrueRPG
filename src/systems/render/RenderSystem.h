#ifndef RPG_RENDERSYSTEM_H
#define RPG_RENDERSYSTEM_H

#include "entt.hpp"
#include "../../client/graphics/SpriteBatch.h"
#include "../../components/basic/TransformComponent.h"
#include "IRenderSubsystem.h"
#include "../../scene/ISystem.h"
#include "../../utils/Types.h"
#include "ILightRenderSubsystem.h"

struct GBuffer
{
    unsigned int id;

    unsigned int gPosition;
    unsigned int gAlbedoSpec;
};

class RenderSystem : public ISystem
{
    entt::registry &m_registry;
    Shader m_shader;
    Shader m_uiShader;
    SpriteBatch m_batch;

    GBuffer m_gBuffer;

    std::vector<IRenderSubsystem *> m_subsystems;
    std::vector<ILightRenderSubsystem *> m_lightSubsystems;
    std::vector<IRenderSubsystem *> m_uiSubsystems;

public:
    RenderSystem(entt::registry &registry);

    ~RenderSystem() override;

    template <typename T>
    decltype(auto) addSubsystem()
    {
        m_subsystems.push_back(new T(m_registry));
        return (T &)*m_subsystems.back();
    }

    template <typename T>
    decltype(auto) addLightSubsystem()
    {
        m_lightSubsystems.push_back(new T(m_registry));
        return (T &)*m_lightSubsystems.back();
    }

    template <typename T>
    decltype(auto) addUiSubsystem()
    {
        m_uiSubsystems.push_back(new T(m_registry));
        return (T &)*m_uiSubsystems.back();
    }

    void draw();

    void update(float deltaTime) override;

    void destroy() override;

    void resize(int width, int height);

private:
    void createGBuffer(int width, int height);
};

#endif // RPG_RENDERSYSTEM_H
