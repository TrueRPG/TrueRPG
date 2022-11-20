#include "../../../../pch.h"
#include <vulkan/vulkan.h>
#include "DeviceBuilder.h"
#include "../Instance.h"

namespace vk
{

    namespace error
    {
        enum class DeviceError
        {
            OUT_OF_HOST_MEMORY,
            OUT_OF_DEVICE_MEMORY,
            INITIALIZATION_FAILED,
            EXTENSION_NOT_PRESENT,
            FEATURE_NOT_PRESENT,
            TOO_MANY_OBJECTS,
            DEVICE_LOST
        };

        std::string toString(DeviceError e)
        {
            switch (e)
            {
            case DeviceError::OUT_OF_HOST_MEMORY:
                return "out of host memory";
            case DeviceError::OUT_OF_DEVICE_MEMORY:
                return "out of device memory";
            case DeviceError::INITIALIZATION_FAILED:
                return "initialization failed";
            case DeviceError::EXTENSION_NOT_PRESENT:
                return "extension not present";
            case DeviceError::FEATURE_NOT_PRESENT:
                return "feature not present";
            case DeviceError::TOO_MANY_OBJECTS:
                return "too many object";
            case DeviceError::DEVICE_LOST:
                return "device lost";
            }

            return "initialization failed";
        }

        struct DeviceErrorCategory : std::error_category
        {
            [[nodiscard]] const char *name() const noexcept override { return "vk_instance"; }
            [[nodiscard]] std::string message(int err) const override { return toString(static_cast<DeviceError>(err)); }
        };
        const static DeviceErrorCategory deviceErrorCategory;

        Error<VkResult> makeError(DeviceError error, VkResult result)
        {
            return {{static_cast<int>(error), deviceErrorCategory}, result};
        }

        DeviceError resultToError(VkResult result)
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
    }

    DeviceBuilder::DeviceBuilder(const Instance &instance) : m_instance(instance)
    {}

    Result<Device> DeviceBuilder::build() const
    {

    }

} // namespace vk