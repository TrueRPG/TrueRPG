#include "LuaScriptApi.h"

#include "OpenSimplexNoise.h"

#include "../scene/components/NameComponent.h"
#include "../scene/components/TransformComponent.h"
#include "../scene/components/CameraComponent.h"
#include "../scene/components/RectColliderComponent.h"
#include "../scene/components/RigidbodyComponent.h"
#include "../scene/components/HierarchyComponent.h"
#include "../scene/components/TextRendererComponent.h"
#include "../scene/components/AudioSourceComponent.h"
#include "../scene/components/AutoOrderComponent.h"

#include "../scene/utils/Hierarchy.h"
#include "../client/audio/StreamAudioClip.h"
#include "../client/audio/CachedAudioClip.h"

namespace LuaApi
{
    void initGlmApi(sol::state &ctx)
    {
        sol::table glmTable = ctx.create_table("glm");

        glmTable["ortho"] = sol::overload(
                sol::resolve<glm::mat4(float, float, float, float)>(glm::ortho<float>),
                sol::resolve<glm::mat4(float, float, float, float, float, float)>(glm::ortho<float>));

        glmTable["perspective"] = sol::resolve<glm::mat4(float, float, float, float)>(glm::perspective<float>);

        glmTable["translate"] = sol::resolve<glm::mat4(const glm::mat4 &, const glm::vec3 &)>(glm::translate);

        glmTable["scale"] = sol::resolve<glm::mat4(const glm::mat4 &, const glm::vec3 &)>(glm::scale);

        glmTable["rotate"] = sol::resolve<glm::mat4(const glm::mat4 &, float, const glm::vec3 &)>(glm::rotate);

        glmTable.new_usertype<glm::vec2>("vec2",
                                         sol::constructors<glm::vec2(), glm::vec2(float), glm::vec2(float, float)>(),
                                         "x", &glm::vec2::x,
                                         "y", &glm::vec2::y,
                                         sol::meta_method::addition, glmAddFunction<glm::vec2, float>(),
                                         sol::meta_method::subtraction, glmSubFunction<glm::vec2, float>(),
                                         sol::meta_method::multiplication, glmMulFunction<glm::vec2, float>(),
                                         sol::meta_method::division, glmDivFunction<glm::vec2, float>(),
                                         sol::meta_method::index, glmIndexOperator<glm::vec2, float>());

        glmTable.new_usertype<glm::ivec2>("ivec2",
                                          sol::constructors<glm::ivec2(), glm::ivec2(int), glm::ivec2(int, int)>(),
                                          "x", &glm::ivec2::x,
                                          "y", &glm::ivec2::y,
                                          sol::meta_method::addition, glmAddFunction<glm::ivec2, int>(),
                                          sol::meta_method::subtraction, glmSubFunction<glm::ivec2, int>(),
                                          sol::meta_method::multiplication, glmMulFunction<glm::ivec2, int>(),
                                          sol::meta_method::division, glmDivFunction<glm::ivec2, int>(),
                                          sol::meta_method::index, glmIndexOperator<glm::ivec2, int>());

        glmTable.new_usertype<glm::vec3>("vec3",
                                         sol::constructors<glm::vec3(), glm::vec3(float), glm::vec3(float, float,
                                                                                                    float)>(),
                                         "x", &glm::vec3::x,
                                         "y", &glm::vec3::y,
                                         "z", &glm::vec3::z,
                                         sol::meta_method::addition, glmAddFunction<glm::vec3, float>(),
                                         sol::meta_method::subtraction, glmSubFunction<glm::vec3, float>(),
                                         sol::meta_method::multiplication, glmMulFunction<glm::vec3, float>(),
                                         sol::meta_method::division, glmDivFunction<glm::vec3, float>(),
                                         sol::meta_method::index, glmIndexOperator<glm::vec3, float>());

        glmTable.new_usertype<glm::ivec3>("ivec3",
                                          sol::constructors<glm::ivec3(), glm::ivec3(int), glm::ivec3(int, int, int)>(),
                                          "x", &glm::ivec3::x,
                                          "y", &glm::ivec3::y,
                                          "z", &glm::ivec3::z,
                                          sol::meta_method::addition, glmAddFunction<glm::ivec3, int>(),
                                          sol::meta_method::subtraction, glmSubFunction<glm::ivec3, int>(),
                                          sol::meta_method::multiplication, glmMulFunction<glm::ivec3, int>(),
                                          sol::meta_method::division, glmDivFunction<glm::ivec3, int>(),
                                          sol::meta_method::index, glmIndexOperator<glm::ivec3, int>());

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
                                         sol::meta_method::index, glmIndexOperator<glm::vec4, float>());

        glmTable.new_usertype<glm::ivec4>("ivec4",
                                          sol::constructors<glm::ivec4(), glm::ivec4(int), glm::ivec4(int, int, int,
                                                                                                      int)>(),
                                          "x", &glm::ivec4::x,
                                          "y", &glm::ivec4::y,
                                          "z", &glm::ivec4::z,
                                          "w", &glm::ivec4::w,
                                          sol::meta_method::addition, glmAddFunction<glm::ivec4, int>(),
                                          sol::meta_method::subtraction, glmSubFunction<glm::ivec4, int>(),
                                          sol::meta_method::multiplication, glmMulFunction<glm::ivec4, int>(),
                                          sol::meta_method::division, glmDivFunction<glm::ivec4, int>(),
                                          sol::meta_method::index, glmIndexOperator<glm::ivec4, int>());

        glmTable.new_usertype<glm::mat4>("mat4",
                                         sol::constructors<glm::mat4(), glm::mat4(float)>(),
                                         sol::meta_method::addition, glmAddFunction<glm::mat4, float>(),
                                         sol::meta_method::subtraction, glmSubFunction<glm::mat4, float>(),
                                         sol::meta_method::multiplication,
                                         sol::overload(
                                                 sol::resolve<glm::mat4(const glm::mat4 &, const glm::mat4 &)>(
                                                         glm::operator*),
                                                 sol::resolve<glm::vec4(const glm::mat4 &, const glm::vec4 &)>(
                                                         glm::operator*),
                                                 sol::resolve<glm::mat4(const glm::mat4 &, const float &)>(
                                                         glm::operator*),
                                                 sol::resolve<glm::mat4(const float &, const glm::mat4 &)>(
                                                         glm::operator*)
                                         ),
                                         sol::meta_method::division,
                                         sol::overload(
                                                 sol::resolve<glm::mat4(const glm::mat4 &, const glm::mat4 &)>(
                                                         glm::operator/),
                                                 sol::resolve<glm::vec4(const glm::mat4 &, const glm::vec4 &)>(
                                                         glm::operator/),
                                                 sol::resolve<glm::mat4(const glm::mat4 &, const float &)>(
                                                         glm::operator/),
                                                 sol::resolve<glm::mat4(const float &, const glm::mat4 &)>(
                                                         glm::operator/)
                                         ),
                                         sol::meta_method::index, glmIndexOperator<glm::mat4, glm::vec4>());
    }

    void initNoiseApi(sol::state &ctx)
    {
        ctx.new_usertype<OpenSimplexNoise>("OpenSimplexNoise",
                                           sol::constructors<
                                                   OpenSimplexNoise(),
                                                   OpenSimplexNoise(u64),
                                                   OpenSimplexNoise(u64, i32),
                                                   OpenSimplexNoise(u64, i32, float),
                                                   OpenSimplexNoise(u64, i32, float, float),
                                                   OpenSimplexNoise(u64, i32, float, float, float)>(),
                                           "seed",
                                           sol::property(&OpenSimplexNoise::getSeed, &OpenSimplexNoise::setSeed),
                                           "octaves",
                                           sol::property(&OpenSimplexNoise::getOctaves, &OpenSimplexNoise::setOctaves),
                                           "lacunarity", sol::property(&OpenSimplexNoise::getLacunarity,
                                                                       &OpenSimplexNoise::setLacunarity),
                                           "persistence", sol::property(&OpenSimplexNoise::getPersistence,
                                                                        &OpenSimplexNoise::setPersistence),
                                           "period",
                                           sol::property(&OpenSimplexNoise::getPeriod, &OpenSimplexNoise::setPeriod),
                                           "getNoise", &OpenSimplexNoise::getNoise);
    }

    void initEntityApi(sol::state &ctx)
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
                                 "getHierarchy", &Entity::getComponent<HierarchyComponent>,
								 "getAudioSource", &Entity::getComponent<AudioSourceComponent>);
    }

    void initComponents(sol::state &ctx)
    {
        ctx.new_enum("HorizontalAlign",
                     "Left", HorizontalAlign::Left,
                     "Right", HorizontalAlign::Right,
                     "Center", HorizontalAlign::Center);

        ctx.new_enum("VerticalAlign",
                     "Top", VerticalAlign::Top,
                     "Bottom", VerticalAlign::Bottom,
                     "Center", VerticalAlign::Center);

        ctx.new_usertype<NameComponent>("NameComponent", "name", &NameComponent::name);

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

        ctx.new_usertype<RigidbodyComponent>("RigidBodyComponent", "velocity", &RigidbodyComponent::velocity);

        ctx.new_usertype<AudioSourceComponent>("AudioSourceComponent",
                                               sol::constructors<AudioSourceComponent(IAudioClip &)>(),
                                               "play", &AudioSourceComponent::play,
                                               "pause", &AudioSourceComponent::pause,
                                               "stop", &AudioSourceComponent::stop);

        ctx.new_usertype<AutoOrderComponent>("AutoOrderComponent",
                                             "orderPivot", &AutoOrderComponent::orderPivot);

        ctx.new_usertype<TextRendererComponent>("TextRendererComponent",
                                                "text", &TextRendererComponent::text,
                                                "color", &TextRendererComponent::color,
                                                "horizontalAlign", &TextRendererComponent::horizontalAlign,
                                                "verticalAlign", &TextRendererComponent::verticalAlign,
                                                "layer", &TextRendererComponent::layer,
                                                "order", &TextRendererComponent::order);
    }

    void initAudioApi(sol::state &ctx)
    {
		sol::table audioTable = ctx.create_table("audio");

		audioTable.new_usertype<StreamAudioClip>("StreamAudioClip",
												 sol::constructors<StreamAudioClip(const std::string &)>(),
												 "getPath", &StreamAudioClip::getPath);
		audioTable.new_usertype<CachedAudioClip>("CachedAudioClip",
												 sol::constructors<CachedAudioClip(const std::string &)>(),
												 "getPath", &CachedAudioClip::getPath);
    }

    void initApi(sol::state &ctx)
    {
        initEntityApi(ctx);
        initNoiseApi(ctx);
        initGlmApi(ctx);
        initAudioApi(ctx);
        initComponents(ctx);
    }
}