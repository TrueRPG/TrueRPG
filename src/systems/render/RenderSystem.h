#ifndef RPG_RENDERSYSTEM_H
#define RPG_RENDERSYSTEM_H

#include "entt.hpp"
#include "../../client/graphics/SpriteBatch.h"
#include "../../components/basic/TransformComponent.h"
#include "IRenderSubsystem.h"
#include "../../scene/ISystem.h"
#include "../../utils/Types.h"

class RenderSystem : public ISystem
{
    entt::registry &m_registry;
    Shader m_shader;
    SpriteBatch m_batch;

    std::vector<IRenderSubsystem *> m_subsystems;

    template <typename T>
    class hasSetShader
    {
    private:
        template <typename C> static i8 test(decltype(&C::setShader));
        template <typename C> static i32 test(...);
    public:
        static constexpr bool value = sizeof(test<T>(0)) == sizeof(i8);
    };

public:
    RenderSystem(entt::registry &registry);

    ~RenderSystem() override;

    template <typename T>
    decltype(auto) addSubsystem()
    {
        m_subsystems.push_back(new T(m_registry));
        if constexpr (hasSetShader<T>::value)
        {
            reinterpret_cast<T *>(m_subsystems.back())->setShader(m_shader);
        }
        return (T &)*m_subsystems.back();
    }

    void draw();

    void update(float deltaTime) override;

    void destroy() override;

    static void resize(int width, int height);
};

#endif // RPG_RENDERSYSTEM_H
