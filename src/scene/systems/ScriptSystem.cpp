#include "ScriptSystem.h"

#include "../components/basic/NativeScriptComponent.h"

ScriptSystem::ScriptSystem(entt::registry &registry)
        : m_registry(registry) {}

void ScriptSystem::update(float deltaTime)
{
    // Update all scripts
    auto view = m_registry.view<NativeScriptComponent>();
    for (auto entity : view)
    {
        auto &nativeScriptComponent = view.get<NativeScriptComponent>(entity);

        // if the scripts aren't created, create them
        if (!nativeScriptComponent.instance)
        {
            nativeScriptComponent.instantiateScript();
            nativeScriptComponent.instance->m_entity = {entity, &m_registry};
            nativeScriptComponent.instance->onCreate();
        }

        nativeScriptComponent.instance->onUpdate(deltaTime);
    }
}

void ScriptSystem::destroyScript(entt::entity entity)
{
    // Destroy the script
    auto &nativeScriptComponent = m_registry.get<NativeScriptComponent>(entity);
    if (!nativeScriptComponent.instance)
    {
        nativeScriptComponent.instance->onDestroy();
        nativeScriptComponent.destroyScript();
    }
}

void ScriptSystem::destroy()
{
    // Destroy all scripts
    auto view = m_registry.view<NativeScriptComponent>();
    for (auto entity : view)
    {
        auto &nativeScriptComponent = view.get<NativeScriptComponent>(entity);
        if (nativeScriptComponent.instance)
        {
            nativeScriptComponent.instance->onDestroy();
            nativeScriptComponent.destroyScript();
        }
    }
}
