#ifndef RPG_ITEMCOMPONENT_H
#define RPG_ITEMCOMPONENT_H

#include "../../client/graphics/OpenGL/Texture.h"
#include "../../client/graphics/Rect.h"

struct ItemComponent
{
    std::string name{};
    std::string description{};

    Texture icon;
    IntRect iconRect{0, 0, 0, 0};
};

#endif // RPG_ITEMCOMPONENT_H
