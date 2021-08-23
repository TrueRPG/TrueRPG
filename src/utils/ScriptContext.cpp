#include "ScriptContext.h"
#include "Types.h"
#include <glm/vec2.hpp>

#include "OpenSimplexNoise.h"

#include "../scene/components/NameComponent.h"
#include "../scene/components/TransformComponent.h"

#include "../scene/utils/Hierarchy.h"
#include "../scene/Entity.h"
#include "sol/sol.hpp"

namespace
{
	enum Components : unsigned int
	{
		NAME,
		TRANSFORM
	};

	template<typename S>
	decltype(auto) glmAddFunction()
	{
		return sol::overload(
				[](const S &v1, const S &v2) -> S { return v1 + v2; });
	}

	template<typename S, typename T>
	decltype(auto) glmSubFunction()
	{
		return sol::overload(
				[](const S &v1, const S &v2) -> S { return v1 - v2; });
	}

	template<typename S, typename T>
	decltype(auto) glmMulFunction()
	{
		return sol::overload(
				[](const S &v1, const S &v2) -> S { return v1 * v2; },
				[](const S &v1, T t) -> S { return v1 * t; },
				[](T t, const S &v1) -> S { return t * v1; } );
	}

	template<typename S, typename T>
	decltype(auto) glmDivFunction()
	{
		return sol::overload(
				[](const S &v1, const S &v2) -> S { return v1 / v2; },
				[](const S &v1, T t) -> S { return v1 / t; },
				[](T t, const S &v1) -> S { return t / v1; } );
	}

    inline void initGlmApi(sol::state &ctx)
	{
        ctx.new_usertype<glm::vec2>("vec2",
				sol::constructors<glm::vec2(), glm::vec2(float), glm::vec2(float, float)>(),
				"x", &glm::vec2::x,
				"y", &glm::vec2::y,
				sol::meta_method::addition, glmAddFunction<glm::vec2>(),
				sol::meta_method::subtraction, glmSubFunction<glm::vec2, float>(),
				sol::meta_method::multiplication, glmMulFunction<glm::vec2, float>(),
				sol::meta_method::division, glmDivFunction<glm::vec2, float>() );	

		ctx.new_usertype<glm::ivec2>("ivec2",
				sol::constructors<glm::ivec2(), glm::ivec2(int), glm::ivec2(int , int)>(),
				"x", &glm::ivec2::x,
				"y", &glm::ivec2::y,
				sol::meta_method::addition, glmAddFunction<glm::ivec2>(),
				sol::meta_method::subtraction, glmSubFunction<glm::ivec2, int>(),
				sol::meta_method::multiplication, glmMulFunction<glm::ivec2, int>(),
				sol::meta_method::division, glmDivFunction<glm::ivec2, int>() );	
	}

	inline void initNoiseApi(sol::state &ctx)
	{
		ctx.new_usertype<OpenSimplexNoise>("OpenSimplexNoise",
				sol::constructors<
					OpenSimplexNoise(), 
					OpenSimplexNoise(u64), 
					OpenSimplexNoise(u64, i32), 
					OpenSimplexNoise(u64, i32, float),
					OpenSimplexNoise(u64, i32, float, float),
					OpenSimplexNoise(u64, i32, float, float, float)>(),
				"seed", sol::property(&OpenSimplexNoise::getSeed, &OpenSimplexNoise::setSeed),
				"octaves", sol::property(&OpenSimplexNoise::getOctaves, &OpenSimplexNoise::setOctaves),
				"lacunarity", sol::property(&OpenSimplexNoise::getLacunarity, &OpenSimplexNoise::setLacunarity),
				"persistence", sol::property(&OpenSimplexNoise::getPersistence, &OpenSimplexNoise::setPersistence),
				"period", sol::property(&OpenSimplexNoise::getPeriod, &OpenSimplexNoise::setPeriod),
				"getNoise", &OpenSimplexNoise::getNoise);
	}

	// TODO: Временно решение. Либо добавить эту функцию как метод в Entity.
	// Либо сделать что то типа реестра компонентов 
	sol::object getComponentEntity(Entity& entity, unsigned int component, sol::this_state s)
	{
		sol::state_view lua(s);
		if (component > Components::TRANSFORM)
			return sol::make_object(lua, sol::nil);

		sol::object componentsList[TRANSFORM + 1] = 
		{
			sol::make_object(lua, entity.getComponent<NameComponent>()),
			sol::make_object(lua, entity.getComponent<TransformComponent>()),
		};

		return componentsList[component];
	}

	inline void initEntityApi(sol::state &ctx)
	{
		sol::table hierarchyTabel = ctx.create_table();
		hierarchyTabel["addChild"] = &Hierarchy::addChild;
		hierarchyTabel["find"] = &Hierarchy::find;
		hierarchyTabel["computeTransform"] = &Hierarchy::computeTransform;
		ctx["Hierarchy"] = hierarchyTabel;

		ctx.new_enum("Components",
				"Name", Components::NAME,
				"Transform", Components::TRANSFORM);

		ctx.new_usertype<Entity>("Entity",	
				"getComponent", &getComponentEntity);
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

	void initApi(sol::state &ctx)
	{
		initEntityApi(ctx);
		initNoiseApi(ctx);
		initGlmApi(ctx);
		initComponents(ctx);
	}
};

ScriptContext::ScriptContext() : m_luaState() 
{
	m_luaState.open_libraries(sol::lib::base, sol::lib::math, sol::lib::string);
	::initApi(m_luaState);
}

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

