#include "Light.h"

#include <string>

#include "Shader.h"

#define MAX_LIGHT_SOURCES 101

Light::Light(Shader &shader) : m_shader(shader), m_lightCount(0) {}

void Light::draw()
{
    if (m_lightCount >= MAX_LIGHT_SOURCES) return;
    m_shader.setUniform("lightSources[" + std::to_string(m_lightCount) + "].color", m_color);
    m_shader.setUniform("lightSources[" + std::to_string(m_lightCount) + "].pos", m_pos);
    m_shader.setUniform("lightSources[" + std::to_string(m_lightCount) + "].intensity", m_intensity);
    m_shader.setUniform("lightSources[" + std::to_string(m_lightCount) + "].radius", m_radius);
    ++m_lightCount;
}

void Light::clean()
{
    for (int i = 0; i < MAX_LIGHT_SOURCES; ++i)
    {
        m_shader.setUniform("lightSources[" + std::to_string(i) + "].color", 0.0f, 0.0f, 0.0f);
        m_shader.setUniform("lightSources[" + std::to_string(i) + "].pos", 0.0f, 0.0f);
        m_shader.setUniform("lightSources[" + std::to_string(i) + "].intensity", 0.0f);
        m_shader.setUniform("lightSources[" + std::to_string(i) + "].radius", 0.0f);
    }
    m_lightCount = 0;
}

void Light::setColor(glm::vec3 color)
{
    m_color = color;
}

void Light::setPosition(glm::vec2 position)
{
    m_pos = position;
}

void Light::setIntensity(float intensity)
{
    m_intensity = intensity;
}

void Light::setRadius(float radius)
{
    m_radius = radius;
}