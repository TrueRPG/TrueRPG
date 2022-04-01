#ifndef RPG_GLOBALLIGHTCOMPONENT_H
#define RPG_GLOBALLIGHTCOMPONENT_H

#include "../../../client/graphics/Texture.h"

struct GlobalLightComponent
{
	Texture lightMap;
	glm::vec3 color{1.0f};
	float intensity;
	bool dayNightCycleEnable{false};
};


#endif