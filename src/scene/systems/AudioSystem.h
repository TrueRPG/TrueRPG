#ifndef RPG_AUDIOSYSTEM_H
#define RPG_AUDIOSYSTEM_H

#include "entt.hpp"
#include "../components/audio/AudioSourceComponent.h"
#include "../../client/audio/AudioSource.h"
#include "../../client/audio/AudioDevice.h"
#include "ISystem.h"

class AudioSystem : public ISystem
{
    entt::registry &m_registry;

    std::unordered_map<entt::entity, AudioSource*> m_audioSourceRegistry;

    AudioDevice m_audioDevice;

public:
    AudioSystem(entt::registry &registry);

    void update(float deltaTime) override;

    void destroy() override;

private:
    void onConstruct(entt::registry &registry, entt::entity entity);
    void onDestroy(entt::registry &registry, entt::entity entity);
};

#endif //RPG_AUDIOSYSTEM_H
