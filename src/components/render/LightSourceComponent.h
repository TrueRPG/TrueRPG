#ifndef RPG_LIGHTSOURCECOMPONENT_H
#define RPG_LIGHTSOURCECOMPONENT_H

#include <glm/glm.hpp>

struct LightSourceComponent
{
    glm::vec3 color{0.5f, 0.5f, 0.0f};
    float intensity{1.0f};
    float radius{1.0f};
};

#endif // RPG_LIGHTSOURCECOMPONENT_H
