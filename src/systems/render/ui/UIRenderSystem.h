#ifndef RPG_UIRENDERSYSTEM_H
#define RPG_UIRENDERSYSTEM_H

#include "../../../client/graphics/SpriteBatch.h"
#include "../IRenderSubsystem.h"
#include "IUIRenderSubsystem.h"
#include "entt.hpp"

class UIRenderSystem : public IRenderSubsystem
{
    entt::registry& m_registry;

    std::vector<IUIRenderSubsystem*> m_subsystems;

public:
    UIRenderSystem(entt::registry& registry);

    ~UIRenderSystem() override;

    template <typename T>
    decltype(auto) addSubsystem()
    {
        m_subsystems.push_back(new T(m_registry));
        return (T &)*m_subsystems.back();
    }

    void draw(SpriteBatch& batch) override;

    void update(float deltaTime) override;
};

#endif // RPG_UIRENDERSYSTEM_H
