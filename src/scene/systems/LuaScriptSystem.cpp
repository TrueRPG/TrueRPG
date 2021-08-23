#include <memory>
#include "LuaScriptSystem.h"
#include "../components/LuaScriptComponent.h"

LuaScriptSystem::LuaScriptSystem(entt::registry &registry) 
	: m_registry(registry),
	  m_ctx() { }

void LuaScriptSystem::update(float deltaTime)
{
	auto view = m_registry.view<LuaScriptComponent>();
	for (auto entity : view)
	{
		auto &luaScriptComponent = view.get<LuaScriptComponent>(entity);
		m_ctx.setCurrentScript(luaScriptComponent.scriptName);
		if (!luaScriptComponent.entity)
		{	
			luaScriptComponent.entity = std::make_shared<Entity>(entity, &m_registry);
			m_ctx.init(luaScriptComponent.scriptPath, luaScriptComponent.entity);

            m_ctx.callMethod("onCreate");
		}
        m_ctx.callMethod("onUpdate", deltaTime);

		m_ctx.reset();
	}
}

void LuaScriptSystem::destroyScript(entt::entity entity)
{
	auto &luaScriptComponent = m_registry.get<LuaScriptComponent>(entity);
    if (!luaScriptComponent.entity)
    {
        m_ctx.setCurrentScript(luaScriptComponent.scriptName);
        m_ctx.callMethod("onDestroy");
		m_ctx.collectGarbage();
		m_ctx.reset();
    }
}

void LuaScriptSystem::destroy()
{
	
}
