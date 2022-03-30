#ifndef RPG_UIRENDERSYSTEM_H
#define RPG_UIRENDERSYSTEM_H

#include "entt.hpp"
#include "../../../client/graphics/SpriteBatch.h"
#include "IRenderSystem.h"

class UIRenderSystem : public IRenderSystem
{
    entt::registry& m_registry;

    Texture m_emptyTexture;

public:
    UIRenderSystem(entt::registry& registry);

    virtual void draw(SpriteBatch& batch);

private:
    void drawButtons(SpriteBatch& batch, glm::vec2 cursor);
    void drawInventory(SpriteBatch& batch, glm::vec2 cursor);
};

#endif // RPG_UIRENDERSYSTEM_H
