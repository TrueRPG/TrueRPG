#ifndef RPG_BUTTONRENDERSYSTEM_H
#define RPG_BUTTONRENDERSYSTEM_H

#include "../../../../client/graphics/SpriteBatch.h"
#include "entt.hpp"
#include "IUIRenderSubsystem.h"

class ButtonRenderSystem : public IUIRenderSubsystem
{
    entt::registry& m_registry;

    Texture m_texture;

public:
    ButtonRenderSystem(entt::registry& registry, Texture texture);

    void draw(SpriteBatch& batch, glm::vec2 cursor) override;
};

#endif // RPG_BUTTONRENDERSYSTEM_H
