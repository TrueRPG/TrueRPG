#include "../../pch.h"
#include "glm/ext/matrix_clip_space.hpp"
#include "CameraComponent.h"

#include "../../client/Engine.h"

glm::mat4 CameraComponent::getProjectionMatrix() const
{
    return glm::ortho(-getWidth() / 2, getWidth() / 2,
                      -getHeight() / 2, getHeight() / 2,
                      0.f, 100.f);
}

float CameraComponent::getWidth() const
{
    return (float)Engine::getWindow().getWidth() / zoom;
}

float CameraComponent::getHeight() const
{
    return (float)Engine::getWindow().getHeight() / zoom;
}
