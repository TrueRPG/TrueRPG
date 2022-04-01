#ifndef RPG_GLOBALLIGHTCOMPONENT_H
#define RPG_GLOBALLIGHTCOMPONENT_H

#include "../../../client/graphics/Texture.h"

struct GlobalLightComponent
{
	Texture lightMap;
	glm::vec4 color{1.0f};
	float brightness;
	bool dayNightCycleEnable{false};
};


#endif