#ifndef RPG_PLAYER_H
#define RPG_PLAYER_H

#include <vector>
#include "../scene/Script.h"
#include "../components/physics/RigidbodyComponent.h"
#include "../components/world/HpComponent.h"
#include "../components/world/InventoryComponent.h"
#include "../components/render/CameraComponent.h"
#include "../components/render/TextRendererComponent.h"
#include "../components/animation/SpriteAnimatorComponent.h"
#include "../utils/Hierarchy.h"
#include "../client/Engine.h"
#include "GLFW/glfw3.h"

// TODO: refactor
class PlayerScript : public Script
{
    Entity m_cameraEntity{};
    Entity m_hpEntity{};
    Entity m_spriteEntity{};

public:
    void onCreate() override
    {
        m_cameraEntity = Hierarchy::find(getEntity(), "camera");
        m_hpEntity = Hierarchy::find(getEntity(), "hp");
        m_spriteEntity = Hierarchy::find(getEntity(), "sprite");
    }

    void onUpdate(float deltaTime) override
    {
        auto &textTransform = m_hpEntity.getComponent<TransformComponent>();
        auto &cameraComponent = m_cameraEntity.getComponent<CameraComponent>();

        textTransform.position = glm::vec2(cameraComponent.getWidth() / 2, cameraComponent.getHeight() / 2);
        textTransform.scale = glm::vec2(1 / cameraComponent.zoom);

        auto &hpComponent = getComponent<HpComponent>();
        auto &textRenderer = m_hpEntity.getComponent<TextRendererComponent>();
        textRenderer.text = "HP: " + std::to_string(hpComponent.value);

        // animator
        auto &animator = m_spriteEntity.getComponent<SpriteAnimatorComponent>();
        auto &rigidbody = getComponent<RigidbodyComponent>();
        animator.parameterStorage.at("velocity") = rigidbody.velocity;
    }
};

#endif //RPG_PLAYER_H
