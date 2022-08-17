#ifndef RPG_PLAYERSYSTEM_H
#define RPG_PLAYERSYSTEM_H

#include "../../scene/ISystem.h"
#include "entt.hpp"
#include "../../client/input/IMapper.h"
#include "../../client/input/StringKeyMapper.h"
#include "../../client/input/KeyMappingConfig.h"
#include "../../scene/Entity.h"
#include "glm/glm.hpp"
#include "../../components/audio/AudioSourceComponent.h"

class PlayerSystem : public ISystem
{
    entt::registry &m_registry;

    KeyMappingConfig m_keyMappingConfig;

    std::vector<Key> m_inputStack;

    float m_animationDelay{0.f};
    int m_currentAnimation{3};
    int m_frame{0};

    bool m_pressedHit{false};
    bool m_pressedInventory{false};
    bool m_torchPressed{false};

public:
    PlayerSystem(entt::registry &registry);

    void update(float deltaTime) override;

    void destroy() override;

private:
    void hittingLogic(entt::entity entity);
    void inventoryLogic(entt::entity entity);
    void torchLogic(entt::entity entity);
    glm::ivec2 computeMovement(Key currentKey);
    void updateInput();
    void playAnimation(Entity sprite, glm::ivec2 movement, float deltaTime);
    void playSound(glm::ivec2 movement, AudioSourceComponent& audioSourceComponent);
};

#endif // RPG_PLAYERSYSTEM_H
