#include "glm/ext/matrix_clip_space.hpp"
#include "CameraComponent.h"

#include "../../../client/window/Window.h"

glm::mat4 CameraComponent::getProjectionMatrix() const
{
    return glm::ortho(-getWidth() / 2, getWidth() / 2,
                      -getHeight() / 2, getHeight() / 2,
                      0.f, 100.f);
}

float CameraComponent::getWidth() const
{
    return (float) Window::getInstance().getWidth() / zoom;
}

float CameraComponent::getHeight() const
{
    return (float) Window::getInstance().getHeight() / zoom;
}
