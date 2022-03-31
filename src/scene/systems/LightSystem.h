#ifndef RPG_LIGHTSYSTEM_H
#define RPG_LIGHTSYSTEM_H

#include <entt.hpp>
#include "../../client/graphics/Texture.h"

class LightSystem
{
private:
	entt::registry &m_registry;
	Texture m_lightTexture;
	int m_windowWidth;
	int m_windowHeight;
	bool m_isResized;
public:

	LightSystem(entt::registry &registry);

	void update();

	void resize(int width, int height);

	void destroy();
};

#endif