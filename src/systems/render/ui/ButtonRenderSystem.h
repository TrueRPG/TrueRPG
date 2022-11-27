#ifndef RPG_BUTTONRENDERSYSTEM_H
#define RPG_BUTTONRENDERSYSTEM_H

#include "entt.hpp"
#include "IUIRenderSubsystem.h"

class ISpriteBatch;

class ButtonRenderSystem : public IUIRenderSubsystem
{
    entt::registry& m_registry;

    bool pressed{false};
    bool hovered{false};

public:
    ButtonRenderSystem(entt::registry& registry);

    void draw(ISpriteBatch& batch, glm::vec2 cursor) override;
};

#endif // RPG_BUTTONRENDERSYSTEM_H
