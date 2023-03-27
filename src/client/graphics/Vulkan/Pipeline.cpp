#include "Pipeline.h"
#include "Device.h"

namespace vk
{

Pipeline::Pipeline(VkPipelineLayout layout, VkPipeline pipeline, const Device *device)
    : m_layout(layout),
      m_pipeline(pipeline),
      m_device(device)
{}

void Pipeline::destroy()
{
    vkDestroyPipeline(*m_device, m_pipeline, nullptr);
}

Pipeline::operator VkPipeline() const
{
    return m_pipeline;
}

} // namespace vk