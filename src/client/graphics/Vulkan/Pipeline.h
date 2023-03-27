#ifndef RPG_PIPELINE_H
#define RPG_PIPELINE_H

#include <vulkan/vulkan.h>

namespace vk
{

class Device;

class Pipeline
{
private:
    VkPipelineLayout m_layout = VK_NULL_HANDLE;
    VkPipeline m_pipeline = VK_NULL_HANDLE;

    const Device *m_device = nullptr;
public:

    Pipeline() = default;
    Pipeline(VkPipelineLayout layout, VkPipeline pipeline, const Device *device);

    void destroy();

    operator VkPipeline() const;
};

} // namespace vk

#endif // RPG_PIPELINE_H
