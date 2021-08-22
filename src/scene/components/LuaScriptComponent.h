#ifndef LUASCRIPTCOMPONENT_H
#define LUASCRIPTCOMPONENT_H

#include <string>
#include <memory>
#include "../Entity.h"

struct LuaScriptComponent
{	
	std::shared_ptr<Entity> entity;

	std::string scriptName;
	std::string scriptPath;
};

#endif

