#ifndef RPG_QUEUE_H
#define RPG_QUEUE_H

#include <vulkan/vulkan.h>
#include "../../../pch.h"

struct Queue
{
    std::optional<u32> familyIndex;
    VkQueue queue = VK_NULL_HANDLE;
};

#endif // RPG_QUEUE_H
