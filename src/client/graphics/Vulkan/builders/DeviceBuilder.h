#ifndef RPG_DEVICEBUILDER_H
#define RPG_DEVICEBUILDER_H

#include "../VulkanUtils.h"
#include "../Device.h"
namespace vk
{

class Instance;

class DeviceBuilder
{
private:
    struct Info
    {

    } m_info;

    const Instance &m_instance;
public:

    DeviceBuilder(const Instance &instance);

    Result<Device> build() const;

};

} // namespace vk

#endif // RPG_DEVICEBUILDER_H
