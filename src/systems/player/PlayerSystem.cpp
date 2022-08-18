#include "../../pch.h"
#include "PlayerSystem.h"

#include "../../client/Engine.h"
#include "../../components/player/PlayerComponent.h"
#include "../../components/physics/RigidbodyComponent.h"
#include "../../components/render/SpriteRendererComponent.h"
#include "../../components/world/HpComponent.h"
#include "../../components/world/InventoryComponent.h"
#include "../../components/render/PointLightComponent.h"

// TODO: refactor
PlayerSystem::PlayerSystem(entt::registry &registry)
    : m_registry(registry),
      m_keyMappingConfig(TRUERPG_RES_DIR "/configs/key-mapping.yml")
{
}

void PlayerSystem::update(float deltaTime)
{
    IWindow &window = Engine::getWindow();

    if (window.getKey(m_keyMappingConfig.getExitKey()))
    {
        window.close();
    }

    auto view = m_registry.view<PlayerComponent>();
    for (auto entity : view)
    {
        auto &player = m_registry.get<PlayerComponent>(entity);

        // TODO: just for a test, delete this in the future
        if (m_registry.all_of<HpComponent>(entity))
        {
            hittingLogic(entity);
        }

        if (m_registry.all_of<InventoryComponent>(entity))
        {
            inventoryLogic(entity);
        }

        if (m_registry.all_of<PointLightComponent>(entity))
        {
            torchLogic(entity);
        }

        updateInput();
        Key currentKey = m_inputStack.empty() ? Key::Unknown : m_inputStack.back();

        glm::ivec2 movement = computeMovement(currentKey);

        if (m_registry.all_of<PlayerComponent>(entity))
        {
            auto &rigidbody = m_registry.get<RigidbodyComponent>(entity);
            rigidbody.velocity = glm::vec2(movement) * player.speed * 200.f;
        }

        if (player.sprite && player.sprite.hasComponent<SpriteRendererComponent>())
        {
            playAnimation(player.sprite, movement, deltaTime);
        }

        if (player.steps && player.steps.hasComponent<AudioSourceComponent>())
        {
            playSound(movement, player.steps.getComponent<AudioSourceComponent>());
        }
    }
}

void PlayerSystem::hittingLogic(entt::entity entity)
{
    IWindow &window = Engine::getWindow();

    auto &player = m_registry.get<PlayerComponent>(entity);

    Key key = m_keyMappingConfig.getHitYourselfKey();

    if (window.getKey(key) && !m_pressedHit)
    {
        auto &hpComponent = m_registry.get<HpComponent>(entity);
        hpComponent.value -= 10;
        m_pressedHit = true;

        if (player.sprite && player.sprite.hasComponent<SpriteRendererComponent>())
        {
            auto &renderer = player.sprite.getComponent<SpriteRendererComponent>();
            renderer.color = glm::vec4(1.f, 0.f, 0.f, 1.f);
        }
    }
    if (!window.getKey(key))
    {
        m_pressedHit = false;
        if (player.sprite && player.sprite.hasComponent<SpriteRendererComponent>())
        {
            auto &renderer = player.sprite.getComponent<SpriteRendererComponent>();
            renderer.color = glm::vec4(1.f);
        }
    }
}

void PlayerSystem::inventoryLogic(entt::entity entity)
{
    IWindow &window = Engine::getWindow();

    Key key = m_keyMappingConfig.getInventoryKey();

    // Inventory opening/closing
    auto& inventory = m_registry.get<InventoryComponent>(entity);

    if (window.getKey(key) && !m_pressedInventory)
    {
        inventory.shown = !inventory.shown;
        m_pressedInventory = true;
    }
    else if (!window.getKey(key))
    {
        m_pressedInventory = false;
    }
}

void PlayerSystem::torchLogic(entt::entity entity)
{
    IWindow &window = Engine::getWindow();
    auto& torch = m_registry.get<PointLightComponent>(entity);

    Key key = m_keyMappingConfig.getTorchKey();

    if (window.getKey(key) && !m_torchPressed)
    {
        torch.enabled = !torch.enabled;
        m_torchPressed = true;
    }
    else if (!window.getKey(key))
    {
        m_torchPressed = false;
    }
}

glm::ivec2 PlayerSystem::computeMovement(Key currentKey)
{
    if (currentKey == Key::Unknown)
    {
        return {0, 0};
    }
    if (currentKey == m_keyMappingConfig.getMoveUpKey())
    {
        m_currentAnimation = 0;
        return {0, 1};
    }
    if (currentKey == m_keyMappingConfig.getMoveLeftKey())
    {
        m_currentAnimation = 2;
        return {-1, 0};
    }
    if (currentKey == m_keyMappingConfig.getMoveDownKey())
    {
        m_currentAnimation = 3;
        return {0, -1};
    }
    if (currentKey == m_keyMappingConfig.getMoveRightKey())
    {
        m_currentAnimation = 1;
        return {1, 0};
    }
    return {0, 0};
}

void PlayerSystem::updateInput()
{
    IWindow &window = Engine::getWindow();

    Key keys[] = {
        m_keyMappingConfig.getMoveUpKey(),
        m_keyMappingConfig.getMoveLeftKey(),
        m_keyMappingConfig.getMoveDownKey(),
        m_keyMappingConfig.getMoveRightKey()
    };

    for (const auto &key : keys)
    {
        if (window.getKey(key))
        {
            if (std::find(m_inputStack.begin(), m_inputStack.end(), key) == m_inputStack.end())
            {
                m_inputStack.push_back(key);
            }
        }
        else
        {
            auto newEnd = std::remove_if(m_inputStack.begin(), m_inputStack.end(),
                                         [key](const Key &k){return k == key;});
            m_inputStack.erase(newEnd, m_inputStack.end());
        }
    }
}

void PlayerSystem::playAnimation(Entity sprite, glm::ivec2 movement, float deltaTime)
{
    auto &renderer = sprite.getComponent<SpriteRendererComponent>();

    if (m_animationDelay > 30.f)
    {
        renderer.textureRect = IntRect(m_frame * 32, m_currentAnimation * 32, 32, 32);
        m_frame++;
        m_animationDelay = 0.f;
    }
    m_animationDelay += deltaTime * 200.f;

    if (m_frame > 2)
    {
        m_frame = 0;
    }

    if (movement == glm::ivec2(0.f))
    {
        m_frame = 1;
    }
}

void PlayerSystem::playSound(glm::ivec2 movement, AudioSourceComponent&audioSourceComponent)
{
    if (movement == glm::ivec2(0.f))
    {
        audioSourceComponent.pause();
    }
    else
    {
        audioSourceComponent.play();
    }
}

void PlayerSystem::destroy()
{

}
