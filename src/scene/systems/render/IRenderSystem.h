#ifndef RPG_IRENDERSYSTEM_H
#define RPG_IRENDERSYSTEM_H

#include "../../../client/graphics/SpriteBatch.h"
class IRenderSystem
{
public:
    virtual ~IRenderSystem() = default;

    virtual void draw(SpriteBatch& batch) = 0;
};

#endif // RPG_IRENDERSYSTEM_H
