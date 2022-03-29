#ifndef RPG_TRANSFORMCOMPONENT_H
#define RPG_TRANSFORMCOMPONENT_H

#include "glm/glm.hpp"

struct TransformComponent
{
    glm::vec2 position{};
    glm::vec2 origin{};
    glm::vec2 scale{1.f};
};

#endif //RPG_TRANSFORMCOMPONENT_H
