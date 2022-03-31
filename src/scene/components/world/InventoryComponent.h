#ifndef RPG_INVENTORYCOMPONENT_H
#define RPG_INVENTORYCOMPONENT_H

#include <vector>
#include "../../Entity.h"

struct InventoryComponent
{
    std::vector<std::vector<Entity>> items;
    bool shown{false};
};

#endif // RPG_INVENTORYCOMPONENT_H
