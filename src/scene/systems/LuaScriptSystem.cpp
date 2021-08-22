#include <memory>
#include "LuaScriptSystem.h"
#include "../components/LuaScriptComponent.h"
#include "../components/NameComponent.h"
#include "../../utils/OpenSimplexNoise.h"
#include "../utils/Hierarchy.h"

void testPrintGlob()
{
	printf("1 Hello from c++\n");
}

int testPrintGlob2()
{
	printf("2 Hello from c++\n");
	return 1;
}

LuaScriptSystem::LuaScriptSystem(entt::registry &registry) 
	: m_registry(registry),
	  m_ctx()
{
	m_ctx.m_luaState.open_libraries(sol::lib::base, sol::lib::math, sol::lib::string);	
    m_ctx.addFunction("testPrintGlob", testPrintGlob);
    m_ctx.addFunction("testPrintGlob2", testPrintGlob2);
    m_ctx.addFunction("addChild", Hierarchy::addChild);
    m_ctx.addFunction("find", Hierarchy::find);
}

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

			m_ctx.callFunction("init");
		}
		m_ctx.callFunction("update", deltaTime);

		m_ctx.reset();
	}
}

void LuaScriptSystem::destroyScript()
{
	
}

void LuaScriptSystem::destroy()
{
	
}


