#ifndef RPG_GLOBALLIGHTCOMPONENT_H
#define RPG_GLOBALLIGHTCOMPONENT_H

#include "../../../client/graphics/Texture.h"

struct GlobalLightComponent
{
	glm::vec3 color{1.0f};
	float intensity{0};
    int time{0};
	bool dayNightCycleEnable{false};
};


#endif