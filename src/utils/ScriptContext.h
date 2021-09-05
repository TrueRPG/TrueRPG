#ifndef RPG_SCRIPTCONTEXT_H
#define RPG_SCRIPTCONTEXT_H

#include <limits>
#include <cstdio>
#include <memory>
#include <sol/sol.hpp>
#include <string>
#include <utility>

class Entity;

class ScriptContext
{
private:
	sol::state m_luaState;
	std::string m_currentScript;
public:
	ScriptContext();

	void setCurrentScript(const std::string &name);

	template<typename... Args>
	void callMethod(const std::string &method, Args&&... args);

	template<typename Class, typename... Args>
	[[maybe_unused]] void addType(const std::string &name, Args&&... args);
	
	template<typename T, typename... Args>
	[[maybe_unused]] void addFunction(T&& key, Args&&... args);

    template<typename T>
	[[maybe_unused]] void addGlobalVar(const std::string &name, T &varibale);

	void reset();

	void collectGarbage();

private:
	void init(const std::string &path, const std::shared_ptr<Entity> &entity);
	friend class LuaScriptSystem;
};

template<typename... Args>
void ScriptContext::callMethod(const std::string &method, Args&&... args)
{
	auto scriptTable = m_luaState[m_currentScript];
	sol::function func = scriptTable[method];
	sol::function_result res = func(scriptTable, std::forward<Args>(args)...);
	// if (!res.valid())
	// {
	// 	sol::error R = res;
	// 	fprintf(stderr, "[WARN] Error runing lua %s function %s: %s\n", m_currentScript.c_str(), method.c_str(), R.what());
	// }
}

template<typename Class, typename... Args>
void ScriptContext::addType(const std::string &name, Args&&... args)
{
	m_luaState.new_usertype<Class>(name, std::forward<Args>(args)...);
}

template<typename T, typename... Args>
void ScriptContext::addFunction(T&& key, Args&&... args)
{	
	m_luaState.set_function(std::forward<T>(key), std::forward<Args>(args)...);
}

template<typename T>
void ScriptContext::addGlobalVar(const std::string &name, T &varibale)
{
    m_luaState.set(name, varibale);
}

#endif // RPG_SCRIPTCONTEXT_H

