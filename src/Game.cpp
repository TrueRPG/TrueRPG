#include "pch.h"
#include "Game.h"

#include "systems/script/ScriptSystem.h"
#include "systems/physics/PhysicsSystem.h"
#include "systems/render/RenderSystem.h"
#include "systems/render/SpriteRenderSystem.h"
#include "systems/render/WorldMapRenderSystem.h"
#include "systems/render/ui/UIRenderSystem.h"
#include "systems/render/ui/InventoryRenderSystem.h"
#include "systems/render/ui/ButtonRenderSystem.h"
#include "systems/render/TextRenderSystem.h"
#include "systems/animation/SpriteAnimatorSystem.h"
#include "systems/audio/AudioSystem.h"

#include "scene/Entity.h"

#include "components/render/CameraComponent.h"
#include "components/script/NativeScriptComponent.h"
#include "components/render/SpriteRendererComponent.h"
#include "components/render/TextRendererComponent.h"
#include "components/world/WorldMapComponent.h"
#include "components/audio/AudioListenerComponent.h"
#include "components/physics/RectColliderComponent.h"
#include "components/physics/RigidbodyComponent.h"
#include "components/render/AutoOrderComponent.h"
#include "components/world/HpComponent.h"
#include "components/render/ui/ButtonComponent.h"
#include "components/world/ItemComponent.h"
#include "components/world/InventoryComponent.h"

#include "scripts/PlayerScript.h"
#include "scripts/DebugInfoScript.h"
#include "scripts/WorldMapScript.h"
#include "scripts/PumpkinScript.h"
#include "scripts/BotScript.h"
#include "scripts/ButtonScript.h"

#include "utils/Hierarchy.h"
#include "utils/Animation.h"
#include "systems/render/PointLightRenderSystem.h"
#include "components/render/PointLightComponent.h"
#include "systems/player/PlayerSystem.h"
#include "components/player/PlayerComponent.h"
#include "systems/render/GlobalLightRenderSystem.h"
#include "components/world/ClockComponent.h"
#include "systems/world/ClockSystem.h"
#include "components/world/EnvironmentComponent.h"
#include "systems/world/EnvironmentSystem.h"

Game::Game()
    : m_font(TRUERPG_RES_DIR "/fonts/vt323.ttf", 32),
      m_heroTexture(Texture::create(TRUERPG_RES_DIR "/textures/hero.png")),
      m_baseTexture(Texture::create(TRUERPG_RES_DIR "/textures/base.png")),
      m_steps(TRUERPG_RES_DIR "/audio/steps.mp3"),
      m_music(TRUERPG_RES_DIR "/audio/music.mp3"),
      m_night(TRUERPG_RES_DIR "/audio/night.mp3")
{
    // Add systems
    m_scene.addSystem<ClockSystem>();
    m_scene.addSystem<EnvironmentSystem>();
    m_scene.addSystem<PlayerSystem>();
    m_scene.addSystem<ScriptSystem>();
    m_scene.addSystem<PhysicsSystem>();
    m_scene.addSystem<SpriteAnimatorSystem>();

    // Render systems
    auto& renderSystem = m_scene.addSystem<RenderSystem>();
    renderSystem.addSubsystem<WorldMapRenderSystem>();
    renderSystem.addSubsystem<SpriteRenderSystem>();

    // Light systems
    renderSystem.addLightSubsystem<GlobalLightRenderSystem>();
    renderSystem.addLightSubsystem<PointLightRenderSystem>();

    // UI systems
    auto& uiRenderSystem = renderSystem.addUiSubsystem<UIRenderSystem>();
    uiRenderSystem.addSubsystem<ButtonRenderSystem>();
    uiRenderSystem.addSubsystem<InventoryRenderSystem>();
    renderSystem.addUiSubsystem<TextRenderSystem>();

    m_scene.addSystem<AudioSystem>();

    // Add entities
    m_clockEntity = m_scene.createEntity("globalClock");
    m_clockEntity.addComponent<ClockComponent>();

    Entity worldMapEntity = m_scene.createEntity("worldMap");

    auto &worldTransform = worldMapEntity.getComponent<TransformComponent>();
    worldTransform.scale = glm::vec2(2.f, 2.f);

    auto &worldMap = worldMapEntity.addComponent<WorldMapComponent>();
    worldMapEntity.addComponent<NativeScriptComponent>().bind<WorldMapScript>(m_baseTexture, m_playerEntity);

    m_cameraEntity = m_scene.createEntity("camera");
    m_cameraEntity.addComponent<CameraComponent>();

    // Button
    Entity buttonEntity = m_scene.createEntity("button");
    buttonEntity.getComponent<TransformComponent>().position = {100.f, 100.f};
    auto &button = buttonEntity.addComponent<ButtonComponent>(&m_font, "test");
    button.onClick = [] {
        std::cout << "button was pressed!" << std::endl;
    };
    button.onHover = [](bool hovered) {
        std::cout << "button is " << (hovered ? "hovered" : "not hovered") << std::endl;
    };
    buttonEntity.addComponent<NativeScriptComponent>().bind<ButtonScript>();
    Hierarchy::addChild(m_cameraEntity, buttonEntity);

    // Create an FPS counter
    Entity debugInfoEntity = m_scene.createEntity("debugInfo");
    auto &debugText = debugInfoEntity.addComponent<TextRendererComponent>(&m_font, "");
    debugText.layer = 10;
    auto &fpsTransform = debugInfoEntity.getComponent<TransformComponent>();
    fpsTransform.scale = glm::vec2(0.8f, 0.8f);
    debugInfoEntity.addComponent<NativeScriptComponent>().bind<DebugInfoScript>(m_cameraEntity, m_clockEntity);

    // Create animation
    m_characterAnimator = Animation::loadAnimatorFromFile(TRUERPG_RES_DIR "/animators/character.yml");

    // Create the player
    m_playerEntity = m_scene.createEntity("player");
    auto &playerTransform = m_playerEntity.getComponent<TransformComponent>();
    m_playerEntity.addComponent<AudioListenerComponent>();

    Entity spriteEntity = m_scene.createEntity("sprite");
    auto &heroRenderer = spriteEntity.addComponent<SpriteRendererComponent>(m_heroTexture);
    heroRenderer.layer = 1;
    spriteEntity.addComponent<AutoOrderComponent>();

    Animation::addAnimator(spriteEntity, &m_characterAnimator);

    auto &heroTransform = spriteEntity.getComponent<TransformComponent>();
    heroTransform.scale = glm::vec2(2.f, 2.f);
    heroTransform.origin = glm::vec2(16, 0);

    auto stepsSoundEntity = m_scene.createEntity("stepsSound");
    auto &stepsComponent = stepsSoundEntity.addComponent<AudioSourceComponent>(m_steps);
    stepsComponent.volume = 0.25f;
    stepsComponent.loop = true;

    auto &playerComponent = m_playerEntity.addComponent<PlayerComponent>();
    playerComponent.sprite = spriteEntity;
    playerComponent.steps = stepsSoundEntity;

    auto &playerCollider = m_playerEntity.addComponent<RectColliderComponent>();
    playerCollider.offset = glm::vec2(-16, 0);
    playerCollider.size = glm::vec2(32, 32);
    m_playerEntity.addComponent<RigidbodyComponent>();

    // HP
    auto hpEntity = m_scene.createEntity("hp");
    auto &hpRenderer = hpEntity.addComponent<TextRendererComponent>(&m_font, "HP: 100");
    hpRenderer.horizontalAlign = HorizontalAlign::Right;
    hpRenderer.verticalAlign = VerticalAlign::Top;
    hpRenderer.layer = 10;
    m_playerEntity.addComponent<HpComponent>();

    auto &torch = m_playerEntity.addComponent<PointLightComponent>();
    torch.color = glm::vec3(0.7f, 0.5f, 0.2f);
    torch.radius = 400.f;
    torch.intensity = 1.0f;
    torch.enabled = false;

    Entity nightAmbient = m_scene.createEntity("nightAmbient");
    auto &nightAudio = nightAmbient.addComponent<AudioSourceComponent>(m_night);
    nightAudio.loop = true;
    nightAudio.global = true;

    Entity environment = m_scene.createEntity("environment");
    auto &environmentComponent = environment.addComponent<EnvironmentComponent>();
    environmentComponent.nightAudio = nightAmbient;

    // --------- Inventory ---------
    // Item
    Entity axeItem = m_scene.createEntity("axeItem");
    auto&axeComponent = axeItem.addComponent<ItemComponent>();
    axeComponent.name = "Axe";
    axeComponent.description = "It's a very useful thing when you need to cut down trees or cut off some monster heads.";
    axeComponent.icon = m_baseTexture;
    axeComponent.iconRect = IntRect(163, 41, 24, 24);

    Entity keyItem = m_scene.createEntity("keyItem");
    auto& keyComponent = keyItem.addComponent<ItemComponent>();
    keyComponent.name = "Secret Key";
    keyComponent.description = "Looks like a very old key. What does it open?";
    keyComponent.icon = m_baseTexture;
    keyComponent.iconRect = IntRect(227, 41, 24, 24);

    // Inventory
    auto& inventoryComponent = m_playerEntity.addComponent<InventoryComponent>();
    inventoryComponent.items = {6, std::vector<Entity>(4, Entity())};
    inventoryComponent.items[0][0] = axeItem;
    inventoryComponent.items[1][0] = keyItem;

    // Attach sprite, sound, hp and camera to the player
    Hierarchy::addChild(m_playerEntity, spriteEntity);
    Hierarchy::addChild(m_playerEntity, stepsSoundEntity);
    Hierarchy::addChild(m_playerEntity, hpEntity);
    Hierarchy::addChild(m_playerEntity, debugInfoEntity);
    Hierarchy::addChild(m_playerEntity, m_cameraEntity);

    // Bind the script to the player
    m_playerEntity.addComponent<NativeScriptComponent>().bind<PlayerScript>();

    // Musical pumpkin
    Entity pumpkinEntity = m_scene.createEntity("pumpkin");
    auto &pumpkinRenderer = pumpkinEntity.addComponent<SpriteRendererComponent>(m_baseTexture);
    pumpkinRenderer.textureRect = IntRect(192, 3584, 32, 32);
    pumpkinRenderer.layer = 0;

    auto &pumpkinTransform = pumpkinEntity.getComponent<TransformComponent>();
    pumpkinTransform.position = glm::vec2(384.f - 32, 256.f - 32);
    pumpkinTransform.scale = glm::vec2(2.f, 2.f);
    pumpkinTransform.origin = glm::vec2(16, 16);

    auto &musicComponent = pumpkinEntity.addComponent<AudioSourceComponent>(m_music);
    musicComponent.volume = 1.0f;

    auto &pumpkinLight = pumpkinEntity.addComponent<PointLightComponent>();
    pumpkinLight.color = glm::vec3(1.0, 0.5, 0.0);
    pumpkinLight.radius = 200.f;
    pumpkinLight.intensity = 1.0f;

    // Text setup for the pumpkin
    Entity pumpkinTextEntity = m_scene.createEntity("text");
    auto &pumpkinTextRenderer = pumpkinTextEntity.addComponent<TextRendererComponent>(&m_font);
    pumpkinTextRenderer.horizontalAlign = HorizontalAlign::Center;
    pumpkinTextRenderer.layer = 10;

    auto &pumpkinTextTransform = pumpkinTextEntity.getComponent<TransformComponent>();
    pumpkinTextTransform.scale = glm::vec2(0.5f);

    Hierarchy::addChild(pumpkinEntity, pumpkinTextEntity);

    pumpkinEntity.addComponent<NativeScriptComponent>().bind<PumpkinScript>(m_playerEntity);

    // Barrels
    Entity barrels[3];

    for (int i = 0; i < 3; i++)
    {
        barrels[i] = m_scene.createEntity("barrel" + std::to_string(i));
        auto &barrelRenderer = barrels[i].addComponent<SpriteRendererComponent>(m_baseTexture);
        barrelRenderer.textureRect = IntRect(96, 736, 32, 32);
        barrelRenderer.layer = 1;

        auto &barrelTransform = barrels[i].getComponent<TransformComponent>();
        barrelTransform.position = glm::vec2(128.f + i * 64.f, 384.f );
        barrelTransform.scale = glm::vec2(2.f, 2.f);

        barrels[i].addComponent<RectColliderComponent>().size = glm::vec2(64, 32);
        auto &order = barrels[i].addComponent<AutoOrderComponent>();
    }

    // Bot
    Entity botEntity = m_scene.createEntity("bot");
    botEntity.getComponent<TransformComponent>().position = glm::vec2(0.f, 5 * 64.f);

    Entity botSprite = m_scene.createEntity("sprite");
    auto &botRenderer = botSprite.addComponent<SpriteRendererComponent>(m_heroTexture);
    botRenderer.layer = 1;
    botSprite.addComponent<AutoOrderComponent>();

    Animation::addAnimator(botSprite, &m_characterAnimator);

    auto &botSpriteTransform = botSprite.getComponent<TransformComponent>();
    botSpriteTransform.scale = glm::vec2(2.f, 2.f);
    botSpriteTransform.origin = glm::vec2(16, 0);

    Entity botNameEntity = m_scene.createEntity("name");
    auto& botTextRenderer = botNameEntity.addComponent<TextRendererComponent>(&m_font, "Bot");
    botTextRenderer.horizontalAlign = HorizontalAlign::Center;
    botTextRenderer.layer = 10;
    auto &botNameTransform = botNameEntity.getComponent<TransformComponent>();
    botNameTransform.position = glm::vec2(0.f, 64.f);

    auto &botCollider = botEntity.addComponent<RectColliderComponent>();
    botCollider.offset = glm::vec2(-16, 0);
    botCollider.size = glm::vec2(32, 32);
    botEntity.addComponent<RigidbodyComponent>();

    Hierarchy::addChild(botEntity, botSprite);
    Hierarchy::addChild(botEntity, botNameEntity);

    botEntity.addComponent<NativeScriptComponent>().bind<BotScript>();
}

void Game::update(float deltaTime)
{
    m_scene.update(deltaTime);
}

void Game::destroy()
{
    m_scene.destroy();
    m_font.destroy();
    m_heroTexture.destroy();
    m_baseTexture.destroy();
}
