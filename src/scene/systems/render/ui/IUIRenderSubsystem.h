#ifndef RPG_IUIRENDERSUBSYSTEM_H
#define RPG_IUIRENDERSUBSYSTEM_H

#include "../../../../client/graphics/SpriteBatch.h"

class IUIRenderSubsystem
{
public:
    virtual ~IUIRenderSubsystem() = default;

    virtual void draw(SpriteBatch& batch, glm::vec2 cursor) = 0;
};

#endif // RPG_IUIRENDERSUBSYSTEM_H
