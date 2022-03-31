#ifndef RPG_GLOBALLIGHTCOMPONENT_H
#define RPG_GLOBALLIGHTCOMPONENT_H

#include "../../../client/graphics/Texture.h"

struct GlobalLightComponent
{
	Texture lightMap;
	float brightness;
};


#endif