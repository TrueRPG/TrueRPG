#ifndef RPG_NATIVESCRIPTCOMPONENT_H
#define RPG_NATIVESCRIPTCOMPONENT_H

#include "../Script.h"

struct NativeScriptComponent
{
    Script *instance = nullptr;

    std::function<void()> instantiateScript;
    std::function<void()> destroyScript;

    template<typename T, typename... Args>
    void bind(Args &&... args)
    {
        instantiateScript = [&]() { instance = static_cast<Script*>(new T(std::forward<Args>(args)...)); };
        destroyScript = [&]() { delete instance; instance = nullptr; };
    }
};

#endif //RPG_NATIVESCRIPTCOMPONENT_H
