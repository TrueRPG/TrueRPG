#include "InstanceBuilder.h"
#include "../../../../utils/Logger.h"
#include <algorithm>
#include <iterator>

namespace vk
{

    namespace errors
    {
        enum class InstanceError
        {
            OUT_OF_HOST_MEMORY,
            OUT_OF_DEVICE_MEMORY,
            INITIALIZATION_FAILED,
            LAYER_NOT_PRESENT,
            EXTENSION_NOT_PRESENT,
            INCOMPATIBLE_DRIVER,
            FAILED_PICK_DEVICE
        };

        std::string toString(InstanceError error)
        {
            switch (error)
            {
            case InstanceError::OUT_OF_HOST_MEMORY:
                return "out of host memory";
            case InstanceError::OUT_OF_DEVICE_MEMORY:
                return "out of device memory";
            case InstanceError::INITIALIZATION_FAILED:
                return "initialization failed";
            case InstanceError::LAYER_NOT_PRESENT:
                return "layer not present";
            case InstanceError::EXTENSION_NOT_PRESENT:
                return "extension not present";
            case InstanceError::INCOMPATIBLE_DRIVER:
                return "incompatible driver";
            case InstanceError::FAILED_PICK_DEVICE:
                return "failed pick physical device";
            }

            return "undefined error";
        }

        struct InstanceErrorCategory : std::error_category
        {
            [[nodiscard]] const char *name() const noexcept override { return "vk_instance"; }
            [[nodiscard]] std::string message(int err) const override { return toString(static_cast<InstanceError>(err)); }
        };
        const static InstanceErrorCategory instanceErrorCategory;

        Error<VkResult> makeError(InstanceError error, VkResult result)
        {
            return {{static_cast<int>(error), instanceErrorCategory}, result};
        }

        InstanceError resultToError(VkResult result)
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
    }

    VkResult createDebugUtilsMessengerEXT(VkInstance instance,
        const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
        const VkAllocationCallbacks* pAllocator,
        VkDebugUtilsMessengerEXT* pDebugMessenger)
    {
        auto func = (PFN_vkCreateDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
        if (func != nullptr)
            return func(instance, pCreateInfo, pAllocator, pDebugMessenger);

        return VK_ERROR_EXTENSION_NOT_PRESENT;
    }

    void destroyDebugUtilsMessengerEXT(VkInstance instance,
        VkDebugUtilsMessengerEXT debugMessenger,
        const VkAllocationCallbacks* pAllocator)
    {
        auto func = (PFN_vkDestroyDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
        if (func != nullptr)
            func(instance, debugMessenger, pAllocator);
    }

    InstanceBuilder &InstanceBuilder::setAppName(const char *name)
    {
        m_info.appName = name;
        return *this;
    }

    InstanceBuilder &InstanceBuilder::setEngineName(const char *name)
    {
        m_info.engineName = name;
        return *this;
    }

    InstanceBuilder &InstanceBuilder::setAppVersion(u32 version)
    {
        m_info.appVersion = version;
        return *this;
    }

    InstanceBuilder &InstanceBuilder::setEngineVersion(u32 version)
    {
        m_info.engineVersion = version;
        return *this;
    }

    InstanceBuilder &InstanceBuilder::setApiVersion(u32 version)
    {
        m_info.apiVersion = version;
        return *this;
    }

    InstanceBuilder &InstanceBuilder::setExtensions(const std::vector<const char *> &extensions)
    {
        std::copy(extensions.begin(), extensions.end(), std::back_inserter(m_info.extensions));
        return *this;
    }

    InstanceBuilder &InstanceBuilder::setLayers(const std::vector<const char *> &layers)
    {
        std::copy(layers.begin(), layers.end(), std::back_inserter(m_info.layers));
        return *this;
    }

    InstanceBuilder &InstanceBuilder::setDebugCallback(PFN_vkDebugUtilsMessengerCallbackEXT callback)
    {
        m_info.debugCallback = callback;
        return *this;
    }

    InstanceBuilder &InstanceBuilder::setDesiredDeviceType(VkPhysicalDeviceType type)
    {
        m_info.desiredType = type;
        return *this;
    }

    Result<Instance> InstanceBuilder::build() const
    {
        u32 deviceCount = 0;
        std::vector<VkPhysicalDevice> physicalDevices;

        VkApplicationInfo appInfo{};
        appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        appInfo.pApplicationName = m_info.appName;
        appInfo.pEngineName = m_info.engineName;
        appInfo.applicationVersion = m_info.appVersion;
        appInfo.engineVersion = m_info.engineVersion;
        appInfo.apiVersion = m_info.apiVersion;

        VkDebugUtilsMessengerCreateInfoEXT debugInfo{};

        VkInstanceCreateInfo info{};
        info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        info.pApplicationInfo = &appInfo;
        info.ppEnabledExtensionNames = m_info.extensions.data();
        info.ppEnabledLayerNames = m_info.layers.data();
        info.enabledExtensionCount = m_info.extensions.size();
        info.enabledLayerCount = m_info.layers.size();

        if constexpr (DEBUG_ENABLED)
        {
            debugInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
            debugInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
                                        VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
            debugInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
                                    VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
            debugInfo.pfnUserCallback = m_info.debugCallback;
            debugInfo.pUserData = nullptr;
            info.pNext = &debugInfo;
        }

        VkInstance instance = VK_NULL_HANDLE;
        VkResult res = vkCreateInstance(&info, nullptr, &instance);

        if (res != VK_SUCCESS)
            return Result<Instance>(errors::makeError(errors::resultToError(res), res));

        logger::debug("Successful create vk instance");

        VkDebugUtilsMessengerEXT debugUtilsMessenger = VK_NULL_HANDLE;

        if constexpr (DEBUG_ENABLED)
        {
            res = createDebugUtilsMessengerEXT(instance, &debugInfo, nullptr, &debugUtilsMessenger);
        }

        vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);
        if (deviceCount <= 0)
            return Result<Instance>(errors::makeError(errors::InstanceError::FAILED_PICK_DEVICE, res));
        physicalDevices.resize(deviceCount);
        vkEnumeratePhysicalDevices(instance, &deviceCount, physicalDevices.data());

        VkPhysicalDevice device = selectSuitablePhysicalDevice(physicalDevices);

        if (device == VK_NULL_HANDLE)
            return Result<Instance>(errors::makeError(errors::InstanceError::FAILED_PICK_DEVICE, res));

        logger::debug("Successful pick physical device");

        Instance result{instance, device, debugUtilsMessenger};

        return Result<Instance>(result);
    }

    VkPhysicalDevice InstanceBuilder::selectSuitablePhysicalDevice(const std::vector<VkPhysicalDevice> &devices) const
    {
        std::multimap<i32, VkPhysicalDevice> candidates;

        for (const auto &device : devices)
        {
            i32 score = ratePhysicalDevice(device);
            candidates.insert(std::make_pair(score, device));
        }

        if (candidates.rbegin()->first > 0)
            return candidates.rbegin()->second;

        return VK_NULL_HANDLE;
    }

    i32 InstanceBuilder::ratePhysicalDevice(VkPhysicalDevice device) const
    {
        i32 score = 0;
        VkPhysicalDeviceProperties deviceProperties;
        VkPhysicalDeviceFeatures deviceFeatures;

        vkGetPhysicalDeviceProperties(device, &deviceProperties);
        vkGetPhysicalDeviceFeatures(device, &deviceFeatures);

        if (deviceProperties.deviceType == m_info.desiredType)
            score += 1000;

        score += static_cast<i32>(deviceProperties.limits.maxImageDimension2D);

        return score;
    }
} // namespace vk
