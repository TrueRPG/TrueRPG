#ifndef RPG_CAMERACOMPONENT_H
#define RPG_CAMERACOMPONENT_H

#include <glm/glm.hpp>

struct CameraComponent
{
    glm::vec4 background{0.2f, 0.3f, 0.3f, 1.0f};
    float zoom{1.f};

    glm::mat4 getProjectionMatrix() const;

    float getWidth() const;

    float getHeight() const;
};

#endif //RPG_CAMERACOMPONENT_H
