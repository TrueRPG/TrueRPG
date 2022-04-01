#include "LightSystem.h"

#include <glm/glm.hpp>

#include "../../client/window/Window.h"
#include "../../client/graphics/Bitmap.h"
#include "../utils/Hierarchy.h"
#include "../components/render/GlobalLightComponent.h"
#include "../components/render/LightSourceComponent.h"
#include "../components/render/CameraComponent.h"

LightSystem::LightSystem(entt::registry &registry) 
        : m_registry(registry),
          m_windowWidth(Window::getInstance().getWidth()),
          m_windowHeight(Window::getInstance().getHeight()),
          m_isResized(true)
{
    Window::getInstance().onResize += createEventHandler(*this, &LightSystem::resize);	  
}

void LightSystem::update(float deltaTime)
{
    Bitmap lightImage(m_windowWidth / 16 + 3, m_windowHeight / 16 + 3);
    GlobalLightComponent *globalLightComponent = nullptr;
    {
        auto view = m_registry.view<GlobalLightComponent>();

        for (auto &entity : view)
        {
            globalLightComponent = &view.get<GlobalLightComponent>(entity);
            if (globalLightComponent->brightness >= 1.0f) return;

            glm::vec2 center(lightImage.getWidth() / 2, lightImage.getHeight() / 2);
            float intensity = 1 - globalLightComponent->brightness;
            float radius = lightImage.getHeight();

            for (size_t x = 0; x < lightImage.getWidth(); ++x)
            {
                for (size_t y = 0; y < lightImage.getHeight(); ++y)
                {
                    float d = glm::distance(center, glm::vec2(x, y));
                    float attenuation = glm::max(0.f, 1 - d / radius);

                    glm::vec4 c = attenuation * attenuation * intensity * globalLightComponent->color;

                    lightImage.setPixel(x, y, {
                        static_cast<unsigned char>(255 * c.r),
                        static_cast<unsigned char>(255 * c.g),
                        static_cast<unsigned char>(255 * c.b),
                        static_cast<unsigned char>(255 * (1 - globalLightComponent->brightness) * (1 - c.a))
                    });
                }
            }
        }
    }

    if (globalLightComponent == nullptr) return;

    CameraComponent *cameraComponent = nullptr;
    TransformComponent cameraTransform;
    {
        auto view = m_registry.view<CameraComponent>();
        for (auto entity : view)
        {
            cameraComponent = &view.get<CameraComponent>(entity);
            cameraTransform = Hierarchy::computeTransform({entity, &m_registry});
        }
    }

    if (cameraComponent != nullptr)
    {
        auto view = m_registry.view<LightSourceComponent>();

        for (auto &entity : view)
        {
            LightSourceComponent &lighSource = view.get<LightSourceComponent>(entity);

            auto transform = Hierarchy::computeTransform({entity, &m_registry});

            glm::mat4 viewMatrix = glm::translate(glm::mat4(1), glm::vec3(-cameraTransform.position, 0));
            glm::vec4 spacePos = cameraComponent->getProjectionMatrix() * (viewMatrix * glm::vec4(transform.position, 0.0f, 1.0f));
            glm::vec3 ndcSpacePos = spacePos / spacePos.w;
            glm::vec2 windowSpacePos = (((glm::vec2(ndcSpacePos) + 1.0f) / 2.0f) * glm::vec2(m_windowWidth, m_windowHeight) + 0.f);
            glm::vec2 textureObjPos = windowSpacePos / 16.0f;

            if (windowSpacePos.x + lighSource.radius * 16 < 0 ||
                windowSpacePos.y + lighSource.radius * 16 < 0 ||
                windowSpacePos.x - lighSource.radius * 16 > m_windowWidth ||
                windowSpacePos.y - lighSource.radius * 16 > m_windowHeight)
                continue;

            float intensity = lighSource.brightness;

            for (size_t x = 0; x < lightImage.getWidth(); ++x)
            {
                for (size_t y = 0; y < lightImage.getHeight(); ++y)
                {
                    float d = glm::distance(textureObjPos, glm::vec2(x, y));
                    float attenuation = glm::max(0.f, 1 - d / lighSource.radius);

                    glm::vec4 c = attenuation * attenuation * intensity * lighSource.color;

                    Pixel currentPixel = lightImage.getPixel(x, y);
                    lightImage.setPixel(x, y, {
                        static_cast<unsigned char>(c.r <= 0.0f ? currentPixel.R : currentPixel.R + (255 * c.r)),
                        static_cast<unsigned char>(c.g <= 0.0f ? currentPixel.G : currentPixel.G + (255 * c.g)),
                        static_cast<unsigned char>(c.b <= 0.0f ? currentPixel.B : currentPixel.B + (255 * c.b)),
                        static_cast<unsigned char>(currentPixel.A * (1 - c.a))
                    });
                }
            }
        }
    }

    if (m_isResized)
    {
        m_lightTexture.destroy();
        m_lightTexture = Texture::create(lightImage);
        globalLightComponent->lightMap = m_lightTexture;
        m_isResized = false;
        return;
    }

    globalLightComponent->lightMap.setData(lightImage);
}

void LightSystem::resize(int width, int height)
{
    m_windowWidth = width;
    m_windowHeight = height;
    m_isResized = true;
}

void LightSystem::destroy()
{
    m_lightTexture.destroy();
}
