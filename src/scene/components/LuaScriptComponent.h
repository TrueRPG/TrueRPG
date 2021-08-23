#ifndef RPG_LUASCRIPTCOMPONENT_H
#define RPG_LUASCRIPTCOMPONENT_H

#include <string>
#include <memory>
#include "../Entity.h"

struct LuaScriptComponent
{	
	std::shared_ptr<Entity> entity;

	std::string scriptName;
	std::string scriptPath;
};

#endif // RPG_LUASCRIPTCOMPONENT_H

