#ifndef RPG_PLAYERCOMPONENT_H
#define RPG_PLAYERCOMPONENT_H

#include "../../client/graphics/OpenGL/Texture.h"
#include "../../scene/Entity.h"

struct PlayerComponent
{
    float speed {1.3f};
    Entity sprite;
    Entity steps;
};

#endif // RPG_PLAYERCOMPONENT_H
