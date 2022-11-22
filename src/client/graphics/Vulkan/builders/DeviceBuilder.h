#ifndef RPG_DEVICEBUILDER_H
#define RPG_DEVICEBUILDER_H

#include "../VulkanUtils.h"
#include "../Device.h"
namespace vk
{

class Instance;
class Surface;

class DeviceBuilder
{
private:
    struct Info
    {
        std::vector<const char*> extensions;
        std::vector<const char*> layers;
    } m_info;

    const Instance &m_instance;
    const Surface &m_surface;
public:

    explicit DeviceBuilder(const Instance &instance, const Surface &surface);

    DeviceBuilder &setExtensions(const std::vector<const char *> &extensions);
    DeviceBuilder &setLayers(const std::vector<const char *> &layers);

    [[nodiscard]] Result<Device> build() const;

};

} // namespace vk

#endif // RPG_DEVICEBUILDER_H
