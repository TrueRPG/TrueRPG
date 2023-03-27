#ifndef RPG_ERROR_H
#define RPG_ERROR_H

#include <vulkan/vulkan.h>
#include "../../../../utils/Result.h"

namespace vk::error
{

enum class InstanceError
{
    OUT_OF_HOST_MEMORY = 1,
    OUT_OF_DEVICE_MEMORY,
    INITIALIZATION_FAILED,
    LAYER_NOT_PRESENT,
    EXTENSION_NOT_PRESENT,
    INCOMPATIBLE_DRIVER,
    FAILED_PICK_DEVICE,
    FAILED_CREATE_DEBUG_MESSENGER
};

enum class DeviceError
{
    OUT_OF_HOST_MEMORY = 1,
    OUT_OF_DEVICE_MEMORY,
    INITIALIZATION_FAILED,
    EXTENSION_NOT_PRESENT,
    FEATURE_NOT_PRESENT,
    TOO_MANY_OBJECTS,
    DEVICE_LOST,
    DONT_SUPPORT_GRAPHICS,
    DONT_SUPPORT_PRESENTATION
};

enum class PipelineError
{
    OUT_OF_HOST_MEMORY = 1,
    OUT_OF_DEVICE_MEMORY,
    INVALID_SHADER_NV,
    FAILED_COMPILE_SHADER,
    INITIALIZATION_FAILED
};

std::string toString(InstanceError error);
std::string toString(DeviceError error);
std::string toString(PipelineError error);

Error<VkResult> makeError(InstanceError error, VkResult result);
Error<VkResult> makeError(DeviceError error, VkResult result);
Error<VkResult> makeError(PipelineError error, VkResult result);

template <typename T>
inline T resultToError(VkResult)
{
    return T::INITIALIZATION_FAILED;
}

template<>
inline InstanceError resultToError<InstanceError>(VkResult result)
{
    switch (result)
    {
    case VK_ERROR_OUT_OF_HOST_MEMORY:
        return InstanceError::OUT_OF_HOST_MEMORY;
    case VK_ERROR_OUT_OF_DEVICE_MEMORY:
        return InstanceError::OUT_OF_DEVICE_MEMORY;
    case VK_ERROR_INITIALIZATION_FAILED:
        return InstanceError::INITIALIZATION_FAILED;
    case VK_ERROR_LAYER_NOT_PRESENT:
        return InstanceError::LAYER_NOT_PRESENT;
    case VK_ERROR_EXTENSION_NOT_PRESENT:
        return InstanceError::EXTENSION_NOT_PRESENT;
    case VK_ERROR_INCOMPATIBLE_DRIVER:
        return InstanceError::INCOMPATIBLE_DRIVER;
    default:
        break;
    }

    return InstanceError::INITIALIZATION_FAILED;
}

template<>
inline DeviceError resultToError<DeviceError>(VkResult result)
{
    switch (result)
    {
    case VK_ERROR_OUT_OF_HOST_MEMORY:
        return DeviceError::OUT_OF_HOST_MEMORY;
    case VK_ERROR_OUT_OF_DEVICE_MEMORY:
        return DeviceError::OUT_OF_DEVICE_MEMORY;
    case VK_ERROR_INITIALIZATION_FAILED:
        return DeviceError::INITIALIZATION_FAILED;
    case VK_ERROR_EXTENSION_NOT_PRESENT:
        return DeviceError::EXTENSION_NOT_PRESENT;
    case VK_ERROR_FEATURE_NOT_PRESENT:
        return DeviceError::FEATURE_NOT_PRESENT;
    case VK_ERROR_TOO_MANY_OBJECTS:
        return DeviceError::TOO_MANY_OBJECTS;
    case VK_ERROR_DEVICE_LOST:
        return DeviceError::DEVICE_LOST;
    default:
        break;
    }

    return DeviceError::INITIALIZATION_FAILED;
}

template<>
inline PipelineError resultToError<PipelineError>(VkResult result)
{
    switch (result)
    {
    case VK_ERROR_OUT_OF_HOST_MEMORY:
        return PipelineError::OUT_OF_HOST_MEMORY;
    case VK_ERROR_OUT_OF_DEVICE_MEMORY:
        return PipelineError::OUT_OF_DEVICE_MEMORY;
    case VK_ERROR_INVALID_SHADER_NV:
        return PipelineError::INVALID_SHADER_NV;
    default:
        break;
    }

    return PipelineError::INITIALIZATION_FAILED;
}

} // namespace vk

#endif // RPG_ERROR_H
