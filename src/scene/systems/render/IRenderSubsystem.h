#ifndef RPG_IRENDERSUBSYSTEM_H
#define RPG_IRENDERSUBSYSTEM_H

#include "../../../client/graphics/SpriteBatch.h"

class IRenderSubsystem
{
public:
    virtual ~IRenderSubsystem() = default;

    virtual void draw(SpriteBatch& batch) = 0;
};

#endif // RPG_IRENDERSUBSYSTEM_H
