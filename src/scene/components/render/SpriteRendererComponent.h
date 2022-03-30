#ifndef RPG_SPRITERENDERERCOMPONENT_H
#define RPG_SPRITERENDERERCOMPONENT_H

#include "glm/glm.hpp"
#include "../../../client/graphics/Texture.h"
#include "../../../client/graphics/Rect.h"

struct SpriteRendererComponent
{
    Texture texture;
    IntRect textureRect{0, 0, 0, 0};
    glm::vec4 color{1.f};

    int layer{0};
    int order{0};

    SpriteRendererComponent(Texture texture);
};

#endif //RPG_SPRITERENDERERCOMPONENT_H
