#ifndef RPG_BUTTONSCRIPT_H
#define RPG_BUTTONSCRIPT_H

#include "../scene/Script.h"
#include "../client/window/Window.h"
#include "../scene/components/render/ui/ButtonComponent.h"

class ButtonScript : public Script
{
public:
    void onUpdate(float deltaTime)
    {
        Window& window = Window::getInstance();

        auto& button = getComponent<ButtonComponent>();
        auto& transform = getComponent<TransformComponent>();
        float indent = window.getWidth() / 80.f;
        transform.position.x = -window.getWidth() / 2.f + indent;
        transform.position.y = window.getHeight() / 2.f - button.size.y - indent;
    }
};

#endif // RPG_BUTTONSCRIPT_H
