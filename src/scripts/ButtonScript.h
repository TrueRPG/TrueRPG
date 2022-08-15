#ifndef RPG_BUTTONSCRIPT_H
#define RPG_BUTTONSCRIPT_H

#include "../scene/Script.h"
#include "../components/render/ui/ButtonComponent.h"
#include "../client/Engine.h"

class ButtonScript : public Script
{
public:
    void onUpdate(float deltaTime)
    {
        IWindow & window = Engine::getWindow();

        auto& button = getComponent<ButtonComponent>();
        auto& transform = getComponent<TransformComponent>();
        float indent = window.getWidth() / 80.f;
        transform.position.x = -window.getWidth() / 2.f + indent;
        transform.position.y = window.getHeight() / 2.f - button.size.y - indent;
    }
};

#endif // RPG_BUTTONSCRIPT_H
