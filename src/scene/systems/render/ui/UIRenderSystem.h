#ifndef RPG_UIRENDERSYSTEM_H
#define RPG_UIRENDERSYSTEM_H

#include "../../../../client/graphics/SpriteBatch.h"
#include "../IRenderSubsystem.h"
#include "IUIRenderSubsystem.h"
#include "entt.hpp"

class UIRenderSystem : public IRenderSubsystem
{
    entt::registry& m_registry;

    std::vector<IUIRenderSubsystem*> m_subsystems;

public:
    UIRenderSystem(entt::registry& registry);

    void addSubsystem(IUIRenderSubsystem& renderSystem);

    void draw(SpriteBatch& batch) override;
};

#endif // RPG_UIRENDERSYSTEM_H
