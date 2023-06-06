#ifndef RPG_CLOCKCOMPONENT_H
#define RPG_CLOCKCOMPONENT_H

#include "../../utils/Clock.h"

struct ClockComponent
{
    Clock clock{50000};
    float clockSpeed{100};
};

#endif // RPG_CLOCKCOMPONENT_H
