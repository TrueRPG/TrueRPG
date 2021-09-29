#include "Game.h"

#include "scene/Entity.h"
#include "scene/components/CameraComponent.h"
#include "scene/components/NativeScriptComponent.h"
#include "scene/components/SpriteRendererComponent.h"
#include "scene/components/TextRendererComponent.h"
#include "scene/components/WorldMapComponent.h"

#include "scene/utils/Hierarchy.h"
#include "scene/utils/Animation.h"
#include "scripts/PlayerScript.h"
#include "scripts/TextScript.h"
#include "scripts/DebugInfoScript.h"
#include "scripts/WorldMapScript.h"
#include "scene/components/AudioListenerComponent.h"
#include "scripts/PumpkinScript.h"
#include "scene/components/RectColliderComponent.h"
#include "scene/components/RigidbodyComponent.h"
#include "scene/components/AutoOrderComponent.h"
#include "scripts/BotScript.h"

Game::Game()
        : m_font("../res/fonts/vt323.ttf", 32),
          m_heroTexture(Texture::create("../res/textures/hero.png")),
          m_baseTexture(Texture::create("../res/textures/base.png")),
          m_steps("../res/audio/steps.mp3"),
          m_music("../res/audio/music.mp3")
{
    Entity worldMapEntity = m_scene.createEntity("worldMap");

    auto &worldTransform = worldMapEntity.getComponent<TransformComponent>();
    worldTransform.scale = glm::vec2(2.f, 2.f);

    auto &worldMap = worldMapEntity.addComponent<WorldMapComponent>();
    worldMapEntity.addComponent<NativeScriptComponent>().bind<WorldMapScript>(m_baseTexture, m_playerEntity);


    m_cameraEntity = m_scene.createEntity("camera");
    m_cameraEntity.addComponent<CameraComponent>();


    // Создание текста
    Entity textEntity = m_scene.createEntity("text");
    auto &textRenderer = textEntity.addComponent<TextRendererComponent>(&m_font, "True RPG!\n Welcome!");

    // Некоторые настройки текста для примера
    textRenderer.horizontalAlign = HorizontalAlign::Center;
    textRenderer.verticalAlign = VerticalAlign::Top;
    textRenderer.layer = 10;

    // Биндим скрипт к энтити и передаем туда камеру
    textEntity.addComponent<NativeScriptComponent>().bind<TextScript>(m_cameraEntity);


    // Создание fps счетчика
    Entity debugInfoEntity = m_scene.createEntity("debugInfo");
    auto &debugText = debugInfoEntity.addComponent<TextRendererComponent>(&m_font, "");
    debugText.layer = 10;
    auto &fpsTransform = debugInfoEntity.getComponent<TransformComponent>();
    fpsTransform.scale = glm::vec2(0.8f, 0.8f);
    debugInfoEntity.addComponent<NativeScriptComponent>().bind<DebugInfoScript>(m_cameraEntity);

    // Создание анимации
    m_characterAnimator = Animation::createAnimator([](SpriteAnimatorBuilder &builder) {
        auto velocity = builder.parameter("velocity", SpriteAnimatorParameterType::Vec2);

        auto idle = builder.node("idle", {{{32, 96, 32, 32}}});
        auto walkLeft =
            builder.node("walkLeft", {{{0, 64, 32, 32}, 0.1f}, {{32, 64, 32, 32}, 0.1f}, {{64, 64, 32, 32}, 0.1f}});
        auto walkRight =
            builder.node("walkRight", {{{0, 32, 32, 32}, 0.1f}, {{32, 32, 32, 32}, 0.1f}, {{64, 32, 32, 32}, 0.1f}});
        auto walkUp =
            builder.node("walkUp", {{{0, 96, 32, 32}, 0.1f}, {{32, 96, 32, 32}, 0.1f}, {{64, 96, 32, 32}, 0.1f}});
        auto walkDown =
            builder.node("walkDown", {{{0, 0, 32, 32}, 0.1f}, {{32, 0, 32, 32}, 0.1f}, {{64, 0, 32, 32}, 0.1f}});

        builder.entry().transition(idle, [](auto) { return true; });

        idle.transition(walkLeft, [=](const auto &storage) { return velocity.get<glm::vec2>(storage).x < 0; });
        walkLeft.transition(idle, [=](const auto &storage) { return velocity.get<glm::vec2>(storage).x >= 0; });

        idle.transition(walkRight, [=](const auto &storage) { return velocity.get<glm::vec2>(storage).x > 0; });
        walkRight.transition(idle, [=](const auto &storage) { return velocity.get<glm::vec2>(storage).x <= 0; });

        idle.transition(walkDown, [=](const auto &storage) { return velocity.get<glm::vec2>(storage).y > 0; });
        walkDown.transition(idle, [=](const auto &storage) { return velocity.get<glm::vec2>(storage).y <= 0; });

        idle.transition(walkUp, [=](const auto &storage) { return velocity.get<glm::vec2>(storage).y < 0; });
        walkUp.transition(idle, [=](const auto &storage) { return velocity.get<glm::vec2>(storage).y >= 0; });
    });

    // Создание игрока
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

    auto &playerCollider = m_playerEntity.addComponent<RectColliderComponent>();
    playerCollider.offset = glm::vec2(-16, 0);
    playerCollider.size = glm::vec2(32, 32);
    m_playerEntity.addComponent<RigidbodyComponent>();

    // Крепим к игроку спрайт, звук, текст и камеру
    Hierarchy::addChild(m_playerEntity, spriteEntity);
    Hierarchy::addChild(m_playerEntity, stepsSoundEntity);
    Hierarchy::addChild(m_playerEntity, textEntity);
    Hierarchy::addChild(m_playerEntity, debugInfoEntity);
    Hierarchy::addChild(m_playerEntity, m_cameraEntity);

    // Биндим скрипт к игроку
    m_playerEntity.addComponent<NativeScriptComponent>().bind<PlayerScript>();


    // Музыкальная тыква
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

    // Настройка текста тыквы
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
