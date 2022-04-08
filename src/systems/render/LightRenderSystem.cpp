#include "LightRenderSystem.h"
#include "../../client/window/Window.h"
#include "../../client/graphics/Light.h"
#include "../../components/render/GlobalLightComponent.h"
#include "../../components/render/LightSourceComponent.h"
#include "../../utils/Types.h"
#include "../../utils/Hierarchy.h"

#define PI 3.14159265358979323846f	/* pi */
#define PI_2 1.57079632679489661923f	/* pi/2 */

LightRenderSystem::LightRenderSystem(entt::registry &registry)
    : m_registry(registry),
      m_shader(),
      m_time(0) {}

void LightRenderSystem::draw(SpriteBatch &batch)
{
    if (m_shader.getId() == 0) return;

    Light light(m_shader);

    drawGlobalLight(light);
    drawLightSources(light, batch);
}

void LightRenderSystem::fixedUpdate()
{
    auto view = m_registry.view<GlobalLightComponent>();

    for (auto entity : view)
    {
        auto &globalLight = view.get<GlobalLightComponent>(entity);

        if (globalLight.dayNightCycleEnable)
        {
            ++globalLight.time;
        }
    }
}

void LightRenderSystem::setShader(Shader shader)
{
    m_shader = std::move(shader);
}

void LightRenderSystem::drawGlobalLight(Light &light)
{
    auto globalLightView = m_registry.view<GlobalLightComponent>();
    if (globalLightView.empty()) return;
    auto &globalLight = globalLightView.get<GlobalLightComponent>(globalLightView[0]);
    light.clean();

    auto &wnd = Window::getInstance();

    m_time = globalLight.time % 24000;
    float ambient = (glm::tanh(4 * glm::sin((m_time / 12000.f) * PI - PI_2)) * 0.5f + 0.5f) * globalLight.intensity;

    light.setColor(globalLight.ambientColor);
    light.setPosition(glm::vec2(wnd.getWidth() / 2.f, wnd.getHeight() / 2.f));
    light.setRadius(wnd.getHeight());
    light.setIntensity(globalLight.intensity);
    m_shader.setUniform("dayTime", globalLight.time);
    m_shader.setUniform("ambient", ambient);

    light.draw();
}

void LightRenderSystem::drawLightSources(Light &light, SpriteBatch &batch)
{
    auto &wnd = Window::getInstance();
    float w = static_cast<float>(wnd.getWidth());
    float h = static_cast<float>(wnd.getHeight());

    auto lightSourcesView = m_registry.view<LightSourceComponent>();

    for (auto entity : lightSourcesView)
    {
        auto &lightSource = lightSourcesView.get<LightSourceComponent>(entity);

        auto transform = Hierarchy::computeTransform({entity, &m_registry});

        glm::vec4 spacePos = batch.getProjectionMatrix() * (batch.getViewMatrix() * glm::vec4(transform.position, 0.0f, 1.0f));
        glm::vec3 ndcSpacePos = spacePos / spacePos.w;
        glm::vec2 windowSpacePos = ((glm::vec2(ndcSpacePos) + 1.0f) / 2.0f) * glm::vec2(w, h);

        if (windowSpacePos.x + lightSource.radius < 0 ||
            windowSpacePos.y + lightSource.radius < 0 ||
            windowSpacePos.x - lightSource.radius > w ||
            windowSpacePos.y - lightSource.radius > h)
        {
            continue;
        }

        float intensity = (-glm::tanh(4 * glm::sin((m_time / 12000.f) * PI - PI_2)) * 0.5f + 0.5f) * lightSource.intensity;

        light.setColor(lightSource.color);
        light.setPosition(windowSpacePos);
        light.setIntensity(intensity);
        light.setRadius(lightSource.radius);
        light.draw();
    }
}