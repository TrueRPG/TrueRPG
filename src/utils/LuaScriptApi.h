#ifndef TRUERPG_LUASCRIPTAPI_H
#define TRUERPG_LUASCRIPTAPI_H

#include <sol/sol.hpp>
#include "Types.h"

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>

namespace LuaApi
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

    template<typename S, typename T>
    decltype(auto) glmAddFunction()
    {
        return sol::resolve<S(const S &, const S &)>(glm::operator+);
    }

    template<typename S, typename T>
    decltype(auto) glmSubFunction()
    {
        return sol::resolve<S(const S &, const S &)>(glm::operator-);
    }

    template<typename S, typename T>
    decltype(auto) glmMulFunction()
    {
        return sol::overload(
                [](const S &v1, const S &v2) -> S { return v1 * v2; },
                [](const S &v, T n) -> S { return v * n; },
                [](T n, const S &v) -> S { return n * v; });
    }

    template<typename S, typename T>
    decltype(auto) glmDivFunction()
    {
        return sol::overload(
                [](const S &v1, const S &v2) -> S { return v1 / v2; },
                [](const S &v, T n) -> S { return v / n; },
                [](T n, const S &v) -> S { return n / v; });
    }

    template<typename S, typename T>
    decltype(auto) glmIndexOperator()
    {
        return [](const S &v, i32 index) -> T { return v[index - 1]; };
    }

    void initGlmApi(sol::state &ctx);

    void initNoiseApi(sol::state &ctx);

    void initEntityApi(sol::state &ctx);

    void initComponents(sol::state &ctx);

    void initAudioApi(sol::state &ctx);

    void initApi(sol::state &ctx);
}

#endif //TRUERPG_LUASCRIPTAPI_H
