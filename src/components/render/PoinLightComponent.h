#ifndef RPG_SRC_COMPONENTS_RENDER_POINLIGHTCOMPONENT_H
#define RPG_SRC_COMPONENTS_RENDER_POINLIGHTCOMPONENT_H

#include "glm/glm.hpp"

struct PointLightComponent
{
    glm::vec3 color;

    float radius;
    float intensity;
};

#endif // RPG_SRC_COMPONENTS_RENDER_POINLIGHTCOMPONENT_H
