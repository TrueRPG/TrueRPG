#ifndef RPG_GLOBALLIGHTCOMPONENT_H
#define RPG_GLOBALLIGHTCOMPONENT_H

#include <glm/glm.hpp>

struct GlobalLightComponent
{
    glm::vec3 ambientColor{1.0f};
    float intensity{0};
    int time{0};
    bool dayNightCycleEnable{false};
};

#endif // RPG_GLOBALLIGHTCOMPONENT_H
