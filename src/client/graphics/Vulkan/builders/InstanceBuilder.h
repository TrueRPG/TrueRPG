#ifndef RPG_INSTANCEBUILDER_H
#define RPG_INSTANCEBUILDER_H

#include <vulkan/vulkan.h>
#include <vector>
#include "../Instance.h"
#include "../../../../utils/Types.h"
#include "../../../../utils/Result.h"

namespace vk
{

template <typename T>
using Result = ::Result<T, Error<VkResult>>;

class InstanceBuilder
{
private:
    struct Info
    {
        const char *appName = "no_name";
        const char *engineName = "no_name";
        u32 appVersion = VK_MAKE_VERSION(1, 0, 0);
        u32 engineVersion = VK_MAKE_VERSION(1, 0, 0);
        u32 apiVersion = VK_API_VERSION_1_1;
        std::vector<const char*> extensions = {};
        std::vector<const char*> layers = {};
        PFN_vkDebugUtilsMessengerCallbackEXT debugCallback = nullptr;
        VkPhysicalDeviceType desiredType = VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU;
    } m_info;
public:
    [[maybe_unused]] InstanceBuilder &setAppName(const char *name);
    [[maybe_unused]] InstanceBuilder &setEngineName(const char *name);
    [[maybe_unused]] InstanceBuilder &setAppVersion(u32 version);
    [[maybe_unused]] InstanceBuilder &setEngineVersion(u32 version);
    [[maybe_unused]] InstanceBuilder &setApiVersion(u32 version);
    [[maybe_unused]] InstanceBuilder &setExtensions(const std::vector<const char*> &extensions);
    [[maybe_unused]] InstanceBuilder &setLayers(const std::vector<const char*> &layers);
    [[maybe_unused]] InstanceBuilder &setDebugCallback(PFN_vkDebugUtilsMessengerCallbackEXT callback);
    [[maybe_unused]] InstanceBuilder &setDesiredDeviceType(VkPhysicalDeviceType type);

    template <u32 version>
    [[maybe_unused]] InstanceBuilder &setAppVersion();
    template <u32 version>
    [[maybe_unused]] InstanceBuilder &setEngineVersion();
    template <u32 version>
    [[maybe_unused]] InstanceBuilder &setApiVersion();

    [[nodiscard]] Result<Instance> build() const;
private:
    VkPhysicalDevice selectSuitablePhysicalDevice(const std::vector<VkPhysicalDevice> &devices) const;
    i32 ratePhysicalDevice(VkPhysicalDevice device) const;
};

#include "InstanceBuilder.inl"

} // namespace vk

#endif // RPG_INSTANCEBUILDER_H
