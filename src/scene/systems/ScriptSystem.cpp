#include "ScriptSystem.h"

#include "../components/NativeScriptComponent.h"

ScriptSystem::ScriptSystem(entt::registry &registry)
        : m_registry(registry) {}

void ScriptSystem::update(float deltaTime)
{
    // Обновляем все скрипты
 	auto view = m_registry.view<NativeScriptComponent>();
    for (auto entity : view)
    {
        auto &nativeScriptComponent = view.get<NativeScriptComponent>(entity);

        // Если они еще не были созданы, то создаем
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
    // Уничтожаем скрипт
    auto &nativeScriptComponent = m_registry.get<NativeScriptComponent>(entity);
    if (!nativeScriptComponent.instance)
    {
        nativeScriptComponent.instance->onDestroy();
        nativeScriptComponent.destroyScript();
    }
}

void ScriptSystem::destroy()
{
    // Уничтожаем все скрипты
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
