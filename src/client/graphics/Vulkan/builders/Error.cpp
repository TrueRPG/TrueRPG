#include "Error.h"

namespace vk::error
{

std::string toString(InstanceError error)
{
    switch (error)
    {
    case InstanceError::OUT_OF_HOST_MEMORY:
        return "vk_instance: out of host memory";
    case InstanceError::OUT_OF_DEVICE_MEMORY:
        return "vk_instance: out of device memory";
    case InstanceError::INITIALIZATION_FAILED:
        return "vk_instance: initialization failed";
    case InstanceError::LAYER_NOT_PRESENT:
        return "vk_instance: layer not present";
    case InstanceError::EXTENSION_NOT_PRESENT:
        return "vk_instance: extension not present";
    case InstanceError::INCOMPATIBLE_DRIVER:
        return "vk_instance: incompatible driver";
    case InstanceError::FAILED_PICK_DEVICE:
        return "vk_instance: failed pick physical device";
    case InstanceError::FAILED_CREATE_DEBUG_MESSENGER:
        return "vk_instance: failed create debug messenger";
    }

    return "undefined error";
}

std::string toString(DeviceError error)
{
    switch (error)
    {
    case DeviceError::OUT_OF_HOST_MEMORY:
        return "vk_device: out of host memory";
    case DeviceError::OUT_OF_DEVICE_MEMORY:
        return "vk_device: out of device memory";
    case DeviceError::INITIALIZATION_FAILED:
        return "vk_device: initialization failed";
    case DeviceError::EXTENSION_NOT_PRESENT:
        return "vk_device: extension not present";
    case DeviceError::FEATURE_NOT_PRESENT:
        return "vk_device: feature not present";
    case DeviceError::TOO_MANY_OBJECTS:
        return "vk_device: too many object";
    case DeviceError::DEVICE_LOST:
        return "vk_device: device lost";
    case DeviceError::DONT_SUPPORT_GRAPHICS:
        return "vk_device: dont support graphics";
    case DeviceError::DONT_SUPPORT_PRESENTATION:
        return "vk_device: dont support presentation";
    }

    return "initialization failed";
}

struct InstanceErrorCategory : std::error_category
{
    [[nodiscard]] const char *name() const noexcept override { return "vk_instance"; }
    [[nodiscard]] std::string message(int err) const override { return toString(static_cast<InstanceError>(err)); }
};
const static InstanceErrorCategory instanceErrorCategory;

struct DeviceErrorCategory : std::error_category
{
    [[nodiscard]] const char *name() const noexcept override { return "vk_device"; }
    [[nodiscard]] std::string message(int err) const override { return toString(static_cast<DeviceError>(err)); }
};
const static DeviceErrorCategory deviceErrorCategory;

Error<VkResult> makeError(InstanceError error, VkResult result)
{
    return {{static_cast<int>(error), instanceErrorCategory}, result};
}

Error<VkResult> makeError(DeviceError error, VkResult result)
{
    return {{static_cast<int>(error), deviceErrorCategory}, result};
}

} // namespace vk