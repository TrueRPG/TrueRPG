#ifndef RPG_LUASCRIPTSYSTEM_H
#define RPG_LUASCRIPTSYSTEM_H

#include <entt.hpp>
#include "../../utils/ScriptContext.h"

class LuaScriptSystem
{
private:
	entt::registry &m_registry;
	ScriptContext m_ctx;
public:	
	explicit LuaScriptSystem(entt::registry &registry);

	void update(float deltaTime);

	void destroyScript(entt::entity entity);

	void destroy();
};

#endif // RPG_LUASCRIPTSYSTEM_H
