#ifndef RPG_INVENTORYCOMPONENT_H
#define RPG_INVENTORYCOMPONENT_H

#include <string>
#include "../../../../client/graphics/Font.h"

struct InventoryComponent
{
    Font *font;
    std::string title;
    glm::ivec2 size{6, 4};

    glm::vec4 cellColor{0.6f, 0.6f, 0.6f, 1.f};
    glm::vec4 panelColor{0.7f, 0.7f, 0.7f, 1.f};

    bool shown{false};

    InventoryComponent(Font *font, std::string text = "Inventory");
};

#endif // RPG_INVENTORYCOMPONENT_H
