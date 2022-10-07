#ifndef RPG_SRC_CLIENT_GRAPHICS_UNIFORMS_H
#define RPG_SRC_CLIENT_GRAPHICS_UNIFORMS_H

#include <glm/glm.hpp>

struct alignas(16) MVPUniform
{
    glm::mat4 model;
    glm::mat4 view;
    glm::mat4 proj;
};

struct LightUniform
{
    alignas(16) glm::vec2 pos;
    alignas(16) glm::vec3 color;

    float radius;
    float intensity;
};

struct alignas(16) GlobalLightUniform
{
    float brightness;
};

#endif // RPG_SRC_CLIENT_GRAPHICS_UNIFORMS_H
