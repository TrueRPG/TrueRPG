#include "ScriptContext.h"
#include <glm/vec2.hpp>

#include "OpenSimplexNoise.h"

#include "../scene/components/NameComponent.h"
#include "../scene/components/TransformComponent.h"

#include "../scene/Entity.h"
#include "sol/sol.hpp"

namespace
{
    inline void initGlmApi(sol::state &ctx)
	{
        ctx.new_usertype<glm::vec2>("Vector2",
				"x", &glm::vec2::x,
				"y", &glm::vec2::y);
	}

	inline void initNoiseApi(sol::state &ctx)
	{
		ctx.new_usertype<OpenSimplexNoise>("OpenSimplexNoise", sol::constructors<OpenSimplexNoise(), OpenSimplexNoise(unsigned int)>(),
			"seed", sol::property(&OpenSimplexNoise::getSeed, &OpenSimplexNoise::setSeed),
			"octaves", sol::property(&OpenSimplexNoise::getOctaves, &OpenSimplexNoise::setOctaves),
			"lacunarity", sol::property(&OpenSimplexNoise::getLacunarity, &OpenSimplexNoise::setLacunarity),
			"persistence", sol::property(&OpenSimplexNoise::getPersistence, &OpenSimplexNoise::setPersistence),
			"period", sol::property(&OpenSimplexNoise::getPeriod, &OpenSimplexNoise::setPeriod),
			"getNoise", &OpenSimplexNoise::getNoise);
	}

	inline void initComponents(sol::state &ctx)
	{
		ctx.new_usertype<NameComponent>("NameComponent",
				"name", &NameComponent::name);
		ctx.new_usertype<TransformComponent>("TransformComponent",
				"position", &TransformComponent::position,
				"scale", &TransformComponent::scale,
				"origin", &TransformComponent::origin);
	}

	inline void initApi(sol::state &ctx)
	{
		ctx.new_usertype<Entity>("Entity",
				"getName", &Entity::getComponent<NameComponent>,
				"getTransform", &Entity::getComponent<TransformComponent>);
		initNoiseApi(ctx);
		initGlmApi(ctx);
		initComponents(ctx);
	}
};

ScriptContext::ScriptContext() : m_luaState() { ::initApi(m_luaState); }

void ScriptContext::setCurrentScript(const  std::string &name)
{
	m_currentTable = name;
}

void ScriptContext::collectGarbage()
{
	m_luaState.collect_garbage();
}

void ScriptContext::reset()
{
	m_currentTable.clear();
}

void ScriptContext::init(const std::string &path, const std::shared_ptr<Entity> &entity)
{
	sol::table scriptTable = m_luaState.require_file(m_currentTable, path); 
	scriptTable["entity"] = entity;
	m_luaState[m_currentTable] = scriptTable;
}

