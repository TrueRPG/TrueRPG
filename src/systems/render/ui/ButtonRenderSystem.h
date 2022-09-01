#ifndef RPG_BUTTONRENDERSYSTEM_H
#define RPG_BUTTONRENDERSYSTEM_H

#include "../../../client/graphics/OpenGL/SpriteBatch.h"
#include "entt.hpp"
#include "IUIRenderSubsystem.h"

class ButtonRenderSystem : public IUIRenderSubsystem
{
    entt::registry& m_registry;

    bool pressed{false};
    bool hovered{false};

public:
    ButtonRenderSystem(entt::registry& registry);

    void draw(SpriteBatch& batch, glm::vec2 cursor) override;
};

#endif // RPG_BUTTONRENDERSYSTEM_H
