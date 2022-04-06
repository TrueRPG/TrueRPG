#include "AudioSystem.h"

#include <iostream>
#include "../../components/audio/AudioListenerComponent.h"
#include "../../utils/Hierarchy.h"

AudioSystem::AudioSystem(entt::registry &registry)
        : m_registry(registry)
{
    // Let's catch the moment of creating/destroying components
    m_registry.on_construct<AudioSourceComponent>().connect<&AudioSystem::onConstruct>(this);
    m_registry.on_destroy<AudioSourceComponent>().connect<&AudioSystem::onDestroy>(this);
}

void AudioSystem::update(float deltaTime)
{
    // Find the listener
    entt::entity listenerEntity = entt::null;
    {
        auto view = m_registry.view<AudioListenerComponent>();
        for (auto entity : view)
        {
            listenerEntity = entity;
            break;
        }
    }

    // If we don't have the listener, do nothing
    if (listenerEntity == entt::null) return;

    // Compute the listener's coordinates
    TransformComponent listenerTransform = Hierarchy::computeTransform({listenerEntity, &m_registry});
    glm::vec2 listenerPosition = listenerTransform.position;

    auto view = m_registry.view<AudioSourceComponent>();
    for (auto entity : view)
    {
        auto &audioSourceComponent = view.get<AudioSourceComponent>(entity);

        // Compute the source's coordinates
        auto transformComponent = Hierarchy::computeTransform({entity, &m_registry});
        glm::vec2 sourcePosition = transformComponent.position;

        // Calculate the volume
        float volumeFactor = 1.f - glm::distance(listenerPosition, sourcePosition) / audioSourceComponent.maxDistance;
        volumeFactor = std::clamp(volumeFactor, 0.f, 1.f);

        // Calculate the panning
        float panFactor = (sourcePosition - listenerPosition).x / audioSourceComponent.maxDistance * 2.f;

        auto* audioSource = m_audioSourceRegistry[entity];
        audioSource->setVolume(audioSourceComponent.volume * volumeFactor);
        audioSource->setPan(audioSourceComponent.pan + panFactor);
        audioSource->setLoop(audioSourceComponent.loop);

        if (audioSourceComponent.state == AudioState::Play)
        {
            audioSource->play();
        }
        else if (audioSourceComponent.state == AudioState::Pause)
        {
            audioSource->pause();
        }
        else if (audioSourceComponent.state == AudioState::Stop)
        {
            audioSource->stop();
        }
    }
}

void AudioSystem::destroy()
{
    // Clear everything here
    m_audioDevice.clear();
    for (const auto &item : m_audioSourceRegistry)
    {
        delete item.second;
    }
    m_audioSourceRegistry.clear();
}

void AudioSystem::onConstruct(entt::registry &registry, entt::entity entity)
{
    // At creation time, we create the corresponding audio source for the component
    // and bind it with the entity in the audio source registry
    auto &audioSourceComponent = registry.get<AudioSourceComponent>(entity);

    auto* audioSource = new AudioSource(*audioSourceComponent.audioClip);
    m_audioSourceRegistry.insert({entity, audioSource});
    m_audioDevice.add(*audioSource);
}

void AudioSystem::onDestroy(entt::registry &registry, entt::entity entity)
{
    // When the component is destroyed, the audio source must be destroyed as well
    auto audioSource = m_audioSourceRegistry[entity];
    m_audioDevice.remove(*audioSource);
    delete audioSource;
    m_audioSourceRegistry.erase(entity);
}
