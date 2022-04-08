#include "ScriptSystem.h"

#include "../../components/script/NativeScriptComponent.h"

ScriptSystem::ScriptSystem(entt::registry &registry)
        : m_registry(registry)
{
    m_registry.on_destroy<NativeScriptComponent>().connect<&ScriptSystem::destroyScript>(this);
}

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

void ScriptSystem::destroyScript(entt::registry &registry, entt::entity entity)
{
    // Destroy the script
    auto &nativeScriptComponent = m_registry.get<NativeScriptComponent>(entity);
    if (!nativeScriptComponent.instance)
    {
        nativeScriptComponent.instance->onDestroy();
        nativeScriptComponent.destroyScript();
    }
}
