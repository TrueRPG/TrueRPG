#ifndef RPG_PIPELINEBUILDER_H
#define RPG_PIPELINEBUILDER_H

#include "../Pipeline.h"
#include "../VulkanUtils.h"
#include "../../PrimitiveType.h"

namespace vk
{

class Device;

class PipelineBuilder
{
private:
    struct Info
    {
        std::string vertexPath;
        std::string fragmentPath;
        std::vector<VkDynamicState> dynamicStates;
        PrimitiveType primitiveType = PrimitiveType::TRIANGEL_LIST;
    } m_info;

    const Device &m_device;
public:
    PipelineBuilder(const Device &device);

    PipelineBuilder &setVertexPath(const std::string &path);
    PipelineBuilder &setFragmentPath(const std::string &path);
    PipelineBuilder &addDynamicState(VkDynamicState state);
    PipelineBuilder &setPrimitiveType(PrimitiveType type);

    ObjResult<Pipeline> build() const;
};

} // namespace vk

#endif // RPG_PIPELINEBUILDER_H
