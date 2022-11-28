#ifndef RPG_SPRITERENDERERCOMPONENT_H
#define RPG_SPRITERENDERERCOMPONENT_H

#include "glm/glm.hpp"
#include "../../client/graphics/Rect.h"

class ITexture;

struct SpriteRendererComponent
{
    ITexture *texture = nullptr;
    IntRect textureRect{0, 0, 0, 0};
    glm::vec4 color{1.f};

    int layer{0};
    int order{0};

    SpriteRendererComponent() = default;
    SpriteRendererComponent(ITexture *texture);
};

#endif //RPG_SPRITERENDERERCOMPONENT_H
