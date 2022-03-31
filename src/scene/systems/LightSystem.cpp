#include "LightSystem.h"

#include <glm/glm.hpp>

#include "../../client/window/Window.h"
#include "../../utils/ImageHelper.h"
#include "../components/render/GlobalLightComponent.h"

LightSystem::LightSystem(entt::registry &registry) 
		: m_registry(registry),
		  m_windowWidth(Window::getInstance().getWidth()),
		  m_windowHeight(Window::getInstance().getHeight()),
		  m_isResized(true)
{
	Window::getInstance().onResize += createEventHandler(*this, &LightSystem::resize);	  
}

void LightSystem::update()
{
	auto view = m_registry.view<GlobalLightComponent>();

	for (auto &entity : view)
	{
		if (m_isResized)
		{
			GlobalLightComponent &globalLightComponent = view.get<GlobalLightComponent>(entity);
			Bitmap lightImage(m_windowWidth, m_windowHeight);
			
			glm::vec4 color(0.0471, 0.0784, 0.271f, 1.0f);
			glm::vec2 center(m_windowWidth / 2, m_windowHeight / 2);
			float intensity = 1 - globalLightComponent.brightness;
			float radius = m_windowHeight;

			for (size_t x = 0; x < lightImage.getWidth(); ++x)
			{
				for (size_t y = 0; y < lightImage.getHeight(); ++y)
				{
					float d = glm::distance(center, glm::vec2(x, y));
					float attenuation = glm::max(0.f, 1 - d / radius);

					glm::vec4 c = attenuation * attenuation * intensity * color;

					lightImage.setPixel(x, y, {
						static_cast<unsigned char>(255 * c.r),
						static_cast<unsigned char>(255 * c.g),
						static_cast<unsigned char>(255 * c.b),
						static_cast<unsigned char>(255 * (1 - c.a) * (1 - globalLightComponent.brightness))
					});
				}
			}

			m_lightTexture.destroy();
			m_lightTexture = Texture::create(lightImage.getRawPixels().data(), m_windowWidth, m_windowHeight);

			globalLightComponent.lightMap = m_lightTexture;

			m_isResized = false;
		}
	}
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
