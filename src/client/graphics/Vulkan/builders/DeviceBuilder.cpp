#include "../../../../pch.h"
#include <vulkan/vulkan.h>
#include "DeviceBuilder.h"
#include "Error.h"
#include "../Instance.h"
#include "../Surface.h"

namespace vk
{
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

    ObjResult<Device> DeviceBuilder::build() const
    {
        u32 queueFamilyCount = 0;
        Queue graphics;
        Queue presentation;
        std::vector<VkQueueFamilyProperties> queueFamilies;
        std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
        VkPhysicalDevice physicalDevice = m_instance.getPhysicalDevice();

        vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, nullptr);
        if (queueFamilyCount <= 0)
            return ObjResult<Device>(error::makeError(error::DeviceError::INITIALIZATION_FAILED, VK_ERROR_INITIALIZATION_FAILED));

        queueFamilies.resize(queueFamilyCount);
        vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, queueFamilies.data());

        for (int i = 0; i < queueFamilies.size(); ++i)
        {
            if (queueFamilies[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)
                graphics.familyIndex = i;

            VkBool32 presentSupport = false;
            vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice, i, m_surface, &presentSupport);

            if (presentSupport)
                presentation.familyIndex = i;

            if (graphics.hasIndex() && presentation.hasIndex())
                break;
        }

        if (!graphics.hasIndex())
            return ObjResult<Device>(error::makeError(error::DeviceError::DONT_SUPPORT_GRAPHICS, VK_ERROR_INITIALIZATION_FAILED));

        if (!presentation.hasIndex())
            return ObjResult<Device>(error::makeError(error::DeviceError::DONT_SUPPORT_PRESENTATION, VK_ERROR_INITIALIZATION_FAILED));

        float priority[] = {1.0f};
        std::set<u32> uniqueQueueFamilies{graphics.familyIndex.value(), presentation.familyIndex.value()};
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
            return ObjResult<Device>(error::makeError(error::resultToError<error::DeviceError>(res), res));

        logger::debug("VkDevice has been created");

        vkGetDeviceQueue(device, graphics.familyIndex.value(), 0, &graphics.queue);
        vkGetDeviceQueue(device, presentation.familyIndex.value(), 0, &presentation.queue);

        return ObjResult<Device>(device, physicalDevice, graphics, presentation);
    }

} // namespace vk