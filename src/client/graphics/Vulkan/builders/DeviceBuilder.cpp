#include "../../../../pch.h"
#include <vulkan/vulkan.h>
#include "DeviceBuilder.h"
#include "../Instance.h"
#include "../Surface.h"

namespace vk
{

    namespace error
    {
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

        std::string toString(DeviceError e)
        {
            switch (e)
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

        struct DeviceErrorCategory : std::error_category
        {
            [[nodiscard]] const char *name() const noexcept override { return "vk_device"; }
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

    DeviceBuilder::DeviceBuilder(const Instance &instance, const Surface &surface) : m_instance(instance), m_surface(surface)
    {}

    DeviceBuilder &DeviceBuilder::setExtensions(const std::vector<const char *> &extensions)
    {
        std::copy(extensions.begin(), extensions.end(), std::back_inserter(m_info.extensions));
        return *this;
    }

    DeviceBuilder &DeviceBuilder::setLayers(const std::vector<const char *> &layers)
    {
        std::copy(layers.begin(), layers.end(), std::back_inserter(m_info.layers));
        return *this;
    }

    Result<Device> DeviceBuilder::build() const
    {
        u32 queueFamilyCount = 0;
        std::optional<u32> graphicsIndex;
        std::optional<u32> presentationIndex;
        std::vector<VkQueueFamilyProperties> queueFamilies;
        std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
        VkPhysicalDevice physicalDevice = m_instance.getPhysicalDevice();

        vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, nullptr);
        if (queueFamilyCount <= 0)
            return Result<Device>(error::makeError(error::DeviceError::INITIALIZATION_FAILED, VK_ERROR_INITIALIZATION_FAILED));

        queueFamilies.resize(queueFamilyCount);
        vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, queueFamilies.data());

        for (int i = 0; i < queueFamilies.size(); ++i)
        {
            if (queueFamilies[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)
                graphicsIndex = i;

            VkBool32 presentSupport = false;
            vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice, i, m_surface, &presentSupport);

            if (presentSupport)
                presentationIndex = i;

            if (graphicsIndex.has_value() && presentationIndex.has_value())
                break;
        }

        if (!graphicsIndex.has_value())
            return Result<Device>(error::makeError(error::DeviceError::DONT_SUPPORT_GRAPHICS, VK_ERROR_INITIALIZATION_FAILED));

        if (!presentationIndex.has_value())
            return Result<Device>(error::makeError(error::DeviceError::DONT_SUPPORT_PRESENTATION, VK_ERROR_INITIALIZATION_FAILED));

        float priority[] = {1.0f};
        std::set<u32> uniqueQueueFamilies{graphicsIndex.value(), presentationIndex.value()};
        for (const auto &index : uniqueQueueFamilies)
        {
            VkDeviceQueueCreateInfo queueCreateInfo{};
            queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
            queueCreateInfo.queueFamilyIndex = index;
            queueCreateInfo.queueCount = 1;
            queueCreateInfo.pQueuePriorities = priority;
            queueCreateInfos.emplace_back(queueCreateInfo);
        }

        VkDeviceCreateInfo info{};
        info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
        info.ppEnabledLayerNames = m_info.layers.data();
        info.enabledLayerCount = m_info.layers.size();
        info.ppEnabledExtensionNames = m_info.extensions.data();
        info.enabledExtensionCount = m_info.extensions.size();
        info.queueCreateInfoCount = queueCreateInfos.size();
        info.pQueueCreateInfos = queueCreateInfos.data();

        VkDevice device = VK_NULL_HANDLE;
        VkResult res = vkCreateDevice(physicalDevice, &info, nullptr, &device);
        if (res != VK_SUCCESS)
            return Result<Device>(error::makeError(error::resultToError(res), res));

        logger::debug("VkDevice has been created");

        return Result<Device>(device, physicalDevice);
    }

} // namespace vk