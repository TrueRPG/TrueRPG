#include "TextRenderSystem.h"
#include "../../components/render/TextRendererComponent.h"
#include "../../../client/graphics/Text.h"
#include "../../utils/Hierarchy.h"

TextRenderSystem::TextRenderSystem(entt::registry &registry)
    : m_registry(registry)
{
}

void TextRenderSystem::draw(SpriteBatch &batch)
{
    auto view = m_registry.view<TextRendererComponent>();
    for (auto entity : view)
    {
        auto &textComponent = view.get<TextRendererComponent>(entity);
        Text text(*textComponent.font, textComponent.text);
        text.setColor(textComponent.color);

        auto transformComponent = Hierarchy::computeTransform({entity, &m_registry});

        text.setPosition(transformComponent.position);
        FloatRect localBound = text.getLocalBounds();
        glm::vec2 textOrigin = transformComponent.origin;
        if (textComponent.horizontalAlign == HorizontalAlign::Center)
        {
            textOrigin += glm::vec2(localBound.getWidth() / 2, 0.f);
        }
        if (textComponent.horizontalAlign == HorizontalAlign::Right)
        {
            textOrigin += glm::vec2(localBound.getWidth(), 0.f);
        }
        if (textComponent.verticalAlign == VerticalAlign::Center)
        {
            textOrigin += glm::vec2(0.f, localBound.getHeight() / 2);
        }
        if (textComponent.verticalAlign == VerticalAlign::Top)
        {
            textOrigin += glm::vec2(0.f, localBound.getHeight());
        }
        text.setOrigin(textOrigin);
        text.setScale(transformComponent.scale);

        text.draw(batch, textComponent.layer, textComponent.order);
    }
}
