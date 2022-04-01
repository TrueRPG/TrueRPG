#ifndef RPG_LIGHTSOURCECOMPONENT_H
#define RPG_LIGHTSOURCECOMPONENT_H

#include <glm/glm.hpp>

struct LightSourceComponent
{
	glm::vec4 color{0.5f, 0.5f, 0.0f, 1.0f};
	float brightness{1.0f};
	float radius{1.0f};
};


#endif