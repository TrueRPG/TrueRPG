#ifndef RPG_SHADERINFO_H
#define RPG_SHADERINFO_H

#include "../../utils/Types.h"

struct ShaderEnabledUniform
{
    bool useMVP;
    bool useLight;
    bool useGlobalLight;
};

#endif // RPG_SHADERINFO_H