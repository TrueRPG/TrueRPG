#ifndef RPG_INVENTORYSCRIPT_H
#define RPG_INVENTORYSCRIPT_H

#include "../scene/Script.h"
#include "../client/window/Window.h"
#include "../scene/components/render/ui/InventoryComponent.h"

class InventoryScript : public Script
{
    bool released;

public:
    void onUpdate(float deltaTime)
    {
        Window& window = Window::getInstance();
        auto& inventory = getComponent<InventoryComponent>();

        if (window.getKey(GLFW_KEY_I) && released)
        {
            inventory.shown = !inventory.shown;
            released = false;
        }
        else if (!window.getKey(GLFW_KEY_I))
        {
            released = true;
        }
    }
};

#endif // RPG_INVENTORYSCRIPT_H
