#include "ScriptContext.h"
#include "LuaScriptApi.h"

ScriptContext::ScriptContext() : m_luaState() 
{
	m_luaState.open_libraries(sol::lib::base, sol::lib::math, sol::lib::string);
	LuaApi::initApi(m_luaState);
}

void ScriptContext::setCurrentScript(const  std::string &name)
{
    m_currentScript = name;
}

void ScriptContext::collectGarbage()
{
	m_luaState.collect_garbage();
}

void ScriptContext::reset()
{
	m_currentScript.clear();
}

void ScriptContext::init(const std::string &path, const std::shared_ptr<Entity> &entity)
{
	sol::table scriptTable = m_luaState.require_file(m_currentScript, path);
	scriptTable["entity"] = entity;
	m_luaState.create_table_with(m_currentScript, scriptTable);
}

