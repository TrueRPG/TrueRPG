#include "../../../../pch.h"
#include "InstanceBuilder.h"
#include <algorithm>
#include <iterator>
#include "Error.h"

namespace vk
{
    static inline VkResult createDebugUtilsMessengerEXT(VkInstance instance,
        const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
        const VkAllocationCallbacks* pAllocator,
        VkDebugUtilsMessengerEXT* pDebugMessenger)
    {
        auto func = (PFN_vkCreateDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
        if (func != nullptr)
            return func(instance, pCreateInfo, pAllocator, pDebugMessenger);

        return VK_ERROR_EXTENSION_NOT_PRESENT;
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

    ObjResult<Instance> InstanceBuilder::build() const
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
            return ObjResult<Instance>(error::makeError(error::resultToError<error::InstanceError>(res), res));

        logger::debug("Successful create vk instance");

        VkDebugUtilsMessengerEXT debugUtilsMessenger = VK_NULL_HANDLE;

        if constexpr (DEBUG_ENABLED)
        {
            res = createDebugUtilsMessengerEXT(instance, &debugInfo, nullptr, &debugUtilsMessenger);

            if (res != VK_SUCCESS)
                return ObjResult<Instance>(error::makeError(error::InstanceError::FAILED_CREATE_DEBUG_MESSENGER, res));
        }

        vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);
        if (deviceCount <= 0)
            return ObjResult<Instance>(error::makeError(error::InstanceError::FAILED_PICK_DEVICE, res));
        physicalDevices.resize(deviceCount);
        vkEnumeratePhysicalDevices(instance, &deviceCount, physicalDevices.data());

        VkPhysicalDevice device = selectSuitablePhysicalDevice(physicalDevices);

        if (device == VK_NULL_HANDLE)
            return ObjResult<Instance>(error::makeError(error::InstanceError::FAILED_PICK_DEVICE, res));

        logger::debug("Successful pick physical device");

        return ObjResult<Instance>(instance, device, debugUtilsMessenger);
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
