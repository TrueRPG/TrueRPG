#ifndef LUASCRIPTSYSTEM_H
#define LUASCRIPTSYSTEM_H

#include <entt.hpp>
#include "../../utils/ScriptContext.h"

class LuaScriptSystem
{
private:
	entt::registry &m_registry;
	ScriptContext m_ctx;
public:	
	LuaScriptSystem(entt::registry &registry);

	void update(float deltaTime);

	void destroyScript(entt::entity entity);

	void destroy();
};

#endif

