#ifndef RPG_SRC_CLIENT_GRAPHICS_LIGHT_H
#define RPG_SRC_CLIENT_GRAPHICS_LIGHT_H

#include <glm/glm.hpp>
#include "Shader.h"

class Light
{
private:
    Shader m_shader;
    glm::vec3 m_color{};
    glm::vec2 m_pos{};
    float m_intensity{};
    float m_radius{};
    int m_lightCount;
public:
    explicit Light(Shader shader);

    void draw();

    void clean();

    void setColor(glm::vec3 color);
    void setPosition(glm::vec2 position);
    void setIntensity(float intensity);
    void setRadius(float radius);
};

#endif // RPG_SRC_CLIENT_GRAPHICS_LIGHT_H
