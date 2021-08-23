#include "ScriptContext.h"
#include "Types.h"
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>

#include "OpenSimplexNoise.h"

#include "../scene/components/NameComponent.h"
#include "../scene/components/TransformComponent.h"
#include "../scene/components/CameraComponent.h"
#include "../scene/components/RectColliderComponent.h"
#include "../scene/components/RigidbodyComponent.h"
#include "../scene/components/HierarchyComponent.h"

#include "../scene/utils/Hierarchy.h"

namespace
{
	enum class Components : unsigned int
	{
		NAME = 0,
		TRANSFORM,
		CAMERA,
		RECT_COLLIDER,
		RIGID_BODY,
		HIERARCHY,
		LAST
	};

	// TODO: не знаю стоит ли переделывать эти функции в макросы. Так что пока inline
	template<typename S, typename T>
	inline __attribute__((always_inline)) decltype(auto) glmAddFunction()
	{
        return sol::resolve<S(const S&, const S&)>(glm::operator+);
	}

	template<typename S, typename T>
	inline __attribute__((always_inline)) decltype(auto) glmSubFunction()
	{
	    return sol::resolve<S(const S&, const S&)>(glm::operator-);
	}

	template<typename S, typename T>
	inline __attribute__((always_inline)) decltype(auto) glmMulFunction()
	{
		return sol::overload(
		        sol::resolve<S(const S&, const S&)>(glm::operator*),
		        sol::resolve<S(const S&, T)>(glm::operator*),
		        sol::resolve<S(T, const S&)>(glm::operator*) );
	}

	template<typename S, typename T>
	inline __attribute__((always_inline)) decltype(auto) glmDivFunction()
	{
	    return sol::overload(
	            sol::resolve<S(const S&, const S&)>(glm::operator/),
	            sol::resolve<S(const S&, T)>(glm::operator/),
	            sol::resolve<S(T, const S&)>(glm::operator/) );
	}

	template<typename S, typename T>
	inline __attribute__((always_inline)) decltype(auto) glmIndexOperator()
	{
	    return [](const S &v, i32 index) -> T { return v[index - 1]; };
	}
	// end

    inline void initGlmApi(sol::state &ctx)
	{
        sol::table glmTable = ctx.create_table("glm");

        glmTable["ortho"] = sol::overload(
                sol::resolve<glm::mat4(float, float, float, float)>(glm::ortho<float>),
                sol::resolve<glm::mat4(float, float, float, float, float, float)>(glm::ortho<float>));

        glmTable["perspective"] = sol::resolve<glm::mat4(float, float, float, float)>(glm::perspective<float>);

        glmTable["translate"] = &glm::translate<float, glm::defaultp>;

        glmTable.new_usertype<glm::vec2>("vec2",
				sol::constructors<glm::vec2(), glm::vec2(float), glm::vec2(float, float)>(),
				"x", &glm::vec2::x,
				"y", &glm::vec2::y,
				sol::meta_method::addition, glmAddFunction<glm::vec2, float>(),
				sol::meta_method::subtraction, glmSubFunction<glm::vec2, float>(),
				sol::meta_method::multiplication, glmMulFunction<glm::vec2, float>(),
				sol::meta_method::division, glmDivFunction<glm::vec2, float>() );	

        glmTable.new_usertype<glm::ivec2>("ivec2",
				sol::constructors<glm::ivec2(), glm::ivec2(int), glm::ivec2(int , int)>(),
				"x", &glm::ivec2::x,
				"y", &glm::ivec2::y,
				sol::meta_method::addition, glmAddFunction<glm::ivec2, int>(),
				sol::meta_method::subtraction, glmSubFunction<glm::ivec2, int>(),
				sol::meta_method::multiplication, glmMulFunction<glm::ivec2, int>(),
				sol::meta_method::division, glmDivFunction<glm::ivec2, int>() );

        glmTable.new_usertype<glm::vec3>("vec3",
                sol::constructors<glm::vec3(), glm::vec3(float), glm::vec3(float, float, float)>(),
                "x", &glm::vec3::x,
                "y", &glm::vec3::y,
                "z", &glm::vec3::z,
                sol::meta_method::addition, glmAddFunction<glm::vec3, float>(),
                sol::meta_method::subtraction, glmSubFunction<glm::vec3, float>(),
                sol::meta_method::multiplication, glmMulFunction<glm::vec3, float>(),
                sol::meta_method::division, glmDivFunction<glm::vec3, float>() );

        glmTable.new_usertype<glm::ivec3>("ivec3",
            sol::constructors<glm::ivec3(), glm::ivec3(int), glm::ivec3(int, int, int)>(),
            "x", &glm::ivec3::x,
            "y", &glm::ivec3::y,
            "z", &glm::ivec3::z,
            sol::meta_method::addition, glmAddFunction<glm::ivec3, int>(),
            sol::meta_method::subtraction, glmSubFunction<glm::ivec3, int>(),
            sol::meta_method::multiplication, glmMulFunction<glm::ivec3, int>(),
            sol::meta_method::division, glmDivFunction<glm::ivec3, int>() );

        glmTable.new_usertype<glm::vec4>("vec4",
                sol::constructors<
                        glm::vec4(),
                        glm::vec4(float),
                        glm::vec4(float, float, float, float),
                        glm::vec4(glm::vec2, float, float),
                        glm::vec4(glm::vec3, float)>(),
                "x", &glm::vec4::x,
                "y", &glm::vec4::y,
                "z", &glm::vec4::z,
                "w", &glm::vec4::w,
                sol::meta_method::addition, glmAddFunction<glm::vec4, float>(),
                sol::meta_method::subtraction, glmSubFunction<glm::vec4, float>(),
                sol::meta_method::multiplication, glmMulFunction<glm::vec4, float>(),
                sol::meta_method::division, glmDivFunction<glm::vec4, float>(),
                sol::meta_method::index, glmIndexOperator<glm::vec4, float>() );

        glmTable.new_usertype<glm::ivec4>("ivec4",
                sol::constructors<glm::ivec4(), glm::ivec4(int), glm::ivec4(int, int, int, int)>(),
                "x", &glm::ivec4::x,
                "y", &glm::ivec4::y,
                "z", &glm::ivec4::z,
                "w", &glm::ivec4::w,
                sol::meta_method::addition, glmAddFunction<glm::ivec4, int>(),
                sol::meta_method::subtraction, glmSubFunction<glm::ivec4, int>(),
                sol::meta_method::multiplication, glmMulFunction<glm::ivec4, int>(),
                sol::meta_method::division, glmDivFunction<glm::ivec4, int>(),
                sol::meta_method::index, glmIndexOperator<glm::ivec4, int>() );

        glmTable.new_usertype<glm::mat4>("mat4",
                sol::constructors<glm::mat4(), glm::mat4(float)>(),
                sol::meta_method::addition, glmAddFunction<glm::mat4, float>(),
                sol::meta_method::subtraction, glmSubFunction<glm::mat4, float>(),
                sol::meta_method::multiplication,
                sol::overload(
                    sol::resolve<glm::mat4(const glm::mat4&, const glm::mat4&)>(glm::operator*),
                    sol::resolve<glm::vec4(const glm::mat4&, const glm::vec4&)>(glm::operator*),
                    sol::resolve<glm::mat4(const glm::mat4&, const float&)>(glm::operator*),
                    sol::resolve<glm::mat4(const float&, const glm::mat4&)>(glm::operator*)
                ),
                sol::meta_method::division,
                sol::overload(
                    sol::resolve<glm::mat4(const glm::mat4&, const glm::mat4&)>(glm::operator/),
                    sol::resolve<glm::vec4(const glm::mat4&, const glm::vec4&)>(glm::operator/),
                    sol::resolve<glm::mat4(const glm::mat4&, const float&)>(glm::operator/),
                    sol::resolve<glm::mat4(const float&, const glm::mat4&)>(glm::operator/)
                ),
                        sol::meta_method::index, glmIndexOperator<glm::mat4, glm::vec4>() );
	}

    // TODO: Нужно сделать что то типа реестра компонентов иначе будет постоянно вылетать т.к. entt не находит
    // компонент и выкидывает SIGABORT
    sol::object getComponentEntity(Entity& entity, unsigned int component, sol::this_state s)
    {
        sol::state_view lua(s);
        if (component > (u64)Components::LAST)
            return sol::make_object(lua, sol::nil);

        sol::object componentsList[(u64)Components::LAST] = {
                sol::make_object(lua, entity.getComponent<NameComponent>()),
                sol::make_object(lua, entity.getComponent<TransformComponent>()),
                sol::make_object(lua, entity.getComponent<CameraComponent>()),
                sol::make_object(lua, entity.getComponent<RectColliderComponent>()),
                sol::make_object(lua, entity.getComponent<RigidbodyComponent>())
        };

        return componentsList[component];
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

	inline void initEntityApi(sol::state &ctx)
	{
	    sol::table hierarchyTable = ctx.create_table("Hierarchy");
        hierarchyTable["addChild"] = &Hierarchy::addChild;
        hierarchyTable["find"] = &Hierarchy::find;
        hierarchyTable["computeTransform"] = &Hierarchy::computeTransform;

		ctx.new_enum("Components",
				"Name", Components::NAME,
				"Transform", Components::TRANSFORM,
				"Camera", Components::CAMERA,
				"RectCollider", Components::RECT_COLLIDER,
				"RigidBody", Components::RIGID_BODY,
				"Hierarchy", Components::HIERARCHY);

		// TODO: пока нельзя нормально создать общий getComponent пока пусть так будет
		ctx.new_usertype<Entity>("Entity",	
				"getName", &Entity::getComponent<NameComponent>,
				"getTransform", &Entity::getComponent<TransformComponent>,
				"getCamera", &Entity::getComponent<CameraComponent>,
				"getRectCollider", &Entity::getComponent<RectColliderComponent>,
				"getRigidBody", &Entity::getComponent<RigidbodyComponent>,
				"getHierarchy", &Entity::getComponent<HierarchyComponent>);
	}

	inline void initComponents(sol::state &ctx)
	{
		ctx.new_usertype<NameComponent>("NameComponent",
				"name", &NameComponent::name);
		ctx.new_usertype<TransformComponent>("TransformComponent",
				"position", &TransformComponent::position,
				"scale", &TransformComponent::scale,
				"origin", &TransformComponent::origin);
		ctx.new_usertype<CameraComponent>("CameraComponent",
                "background", &CameraComponent::background,
                "zoom", &CameraComponent::zoom,
                "getWidth", &CameraComponent::getWidth,
                "getHeight", &CameraComponent::getHeight,
                "getProjectionMatrix", &CameraComponent::getProjectionMatrix);
		ctx.new_usertype<RectColliderComponent>("RectColliderComponent",
                "offset", &RectColliderComponent::offset,
                "size", &RectColliderComponent::size);
		ctx.new_usertype<RigidbodyComponent>("RigidBodyComponent",
                "velocity", &RigidbodyComponent::velocity);
	}

	void initApi(sol::state &ctx)
	{
		initEntityApi(ctx);
		initNoiseApi(ctx);
		initGlmApi(ctx);
		initComponents(ctx);
	}
}

ScriptContext::ScriptContext() : m_luaState() 
{
	m_luaState.open_libraries(sol::lib::base, sol::lib::math, sol::lib::string);
	::initApi(m_luaState);
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

