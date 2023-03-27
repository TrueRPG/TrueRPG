#include "../../../../pch.h"
#include "PipelineBuilder.h"
#include "Error.h"
#include "../Device.h"
#include "../../Vertex.h"
#include <SPIRV/GlslangToSpv.h>
#include <glslang/Public/ShaderLang.h>

namespace vk
{

static constexpr TBuiltInResource getResources() {
    TBuiltInResource resources = {};
    resources.maxLights = 32;
    resources.maxClipPlanes = 6;
    resources.maxTextureUnits = 32;
    resources.maxTextureCoords = 32;
    resources.maxVertexAttribs = 64;
    resources.maxVertexUniformComponents = 4096;
    resources.maxVaryingFloats = 64;
    resources.maxVertexTextureImageUnits = 32;
    resources.maxCombinedTextureImageUnits = 80;
    resources.maxTextureImageUnits = 32;
    resources.maxFragmentUniformComponents = 4096;
    resources.maxDrawBuffers = 32;
    resources.maxVertexUniformVectors = 128;
    resources.maxVaryingVectors = 8;
    resources.maxFragmentUniformVectors = 16;
    resources.maxVertexOutputVectors = 16;
    resources.maxFragmentInputVectors = 15;
    resources.minProgramTexelOffset = -8;
    resources.maxProgramTexelOffset = 7;
    resources.maxClipDistances = 8;
    resources.maxComputeWorkGroupCountX = 65535;
    resources.maxComputeWorkGroupCountY = 65535;
    resources.maxComputeWorkGroupCountZ = 65535;
    resources.maxComputeWorkGroupSizeX = 1024;
    resources.maxComputeWorkGroupSizeY = 1024;
    resources.maxComputeWorkGroupSizeZ = 64;
    resources.maxComputeUniformComponents = 1024;
    resources.maxComputeTextureImageUnits = 16;
    resources.maxComputeImageUniforms = 8;
    resources.maxComputeAtomicCounters = 8;
    resources.maxComputeAtomicCounterBuffers = 1;
    resources.maxVaryingComponents = 60;
    resources.maxVertexOutputComponents = 64;
    resources.maxGeometryInputComponents = 64;
    resources.maxGeometryOutputComponents = 128;
    resources.maxFragmentInputComponents = 128;
    resources.maxImageUnits = 8;
    resources.maxCombinedImageUnitsAndFragmentOutputs = 8;
    resources.maxCombinedShaderOutputResources = 8;
    resources.maxImageSamples = 0;
    resources.maxVertexImageUniforms = 0;
    resources.maxTessControlImageUniforms = 0;
    resources.maxTessEvaluationImageUniforms = 0;
    resources.maxGeometryImageUniforms = 0;
    resources.maxFragmentImageUniforms = 8;
    resources.maxCombinedImageUniforms = 8;
    resources.maxGeometryTextureImageUnits = 16;
    resources.maxGeometryOutputVertices = 256;
    resources.maxGeometryTotalOutputComponents = 1024;
    resources.maxGeometryUniformComponents = 1024;
    resources.maxGeometryVaryingComponents = 64;
    resources.maxTessControlInputComponents = 128;
    resources.maxTessControlOutputComponents = 128;
    resources.maxTessControlTextureImageUnits = 16;
    resources.maxTessControlUniformComponents = 1024;
    resources.maxTessControlTotalOutputComponents = 4096;
    resources.maxTessEvaluationInputComponents = 128;
    resources.maxTessEvaluationOutputComponents = 128;
    resources.maxTessEvaluationTextureImageUnits = 16;
    resources.maxTessEvaluationUniformComponents = 1024;
    resources.maxTessPatchComponents = 120;
    resources.maxPatchVertices = 32;
    resources.maxTessGenLevel = 64;
    resources.maxViewports = 16;
    resources.maxVertexAtomicCounters = 0;
    resources.maxTessControlAtomicCounters = 0;
    resources.maxTessEvaluationAtomicCounters = 0;
    resources.maxGeometryAtomicCounters = 0;
    resources.maxFragmentAtomicCounters = 8;
    resources.maxCombinedAtomicCounters = 8;
    resources.maxAtomicCounterBindings = 1;
    resources.maxVertexAtomicCounterBuffers = 0;
    resources.maxTessControlAtomicCounterBuffers = 0;
    resources.maxTessEvaluationAtomicCounterBuffers = 0;
    resources.maxGeometryAtomicCounterBuffers = 0;
    resources.maxFragmentAtomicCounterBuffers = 1;
    resources.maxCombinedAtomicCounterBuffers = 1;
    resources.maxAtomicCounterBufferSize = 16384;
    resources.maxTransformFeedbackBuffers = 4;
    resources.maxTransformFeedbackInterleavedComponents = 64;
    resources.maxCullDistances = 8;
    resources.maxCombinedClipAndCullDistances = 8;
    resources.maxSamples = 4;
    resources.limits.nonInductiveForLoops = true;
    resources.limits.whileLoops = true;
    resources.limits.doWhileLoops = true;
    resources.limits.generalUniformIndexing = true;
    resources.limits.generalAttributeMatrixVectorIndexing = true;
    resources.limits.generalVaryingIndexing = true;
    resources.limits.generalSamplerIndexing = true;
    resources.limits.generalVariableIndexing = true;
    resources.limits.generalConstantMatrixVectorIndexing = true;
    return resources;
}

static inline EShLanguage getEshLanguage(VkShaderStageFlags stageFlag) {
    switch (stageFlag) {
    case VK_SHADER_STAGE_COMPUTE_BIT:
        return EShLangCompute;
    case VK_SHADER_STAGE_VERTEX_BIT:
        return EShLangVertex;
    case VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT:
        return EShLangTessControl;
    case VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT:
        return EShLangTessEvaluation;
    case VK_SHADER_STAGE_GEOMETRY_BIT:
        return EShLangGeometry;
    case VK_SHADER_STAGE_FRAGMENT_BIT:
        return EShLangFragment;
    default:
        return EShLangCount;
    }
}

static inline std::vector<char> readFile(const std::string &path)
{
    std::vector<char> fileCode;
    std::ifstream file{path, std::ios::ate | std::ios::binary};

    if (!file.is_open())
        throw;

    auto fileSize = static_cast<u32>(file.tellg());
    fileCode.resize(fileSize);

    file.seekg(0);
    file.read(fileCode.data(), fileSize);
    file.close();

    return fileCode;
}

bool compileShader(const Device &device, const std::string &path, VkShaderStageFlags stage, VkShaderModule &module)
{
    if (path.empty())
    {
        logger::warning("path to shader is empty");
        return false;
    }

    auto code = readFile(path);

    glslang::InitializeProcess();
    auto language = getEshLanguage(stage);
    glslang::TProgram program;
    glslang::TShader shader{language};
    auto resource = getResources();

    auto messages = static_cast<EShMessages>(EShMsgSpvRules | EShMsgVulkanRules);
    const char* pCode[1] = {code.data()};

    shader.setStrings(pCode, 1);

    if (!shader.parse(&resource, 100, false, messages))
    {
        logger::info(shader.getInfoLog());
        logger::debug(shader.getInfoDebugLog());
        return false;
    }

    program.addShader(&shader);

    if (!program.link(messages) || !program.mapIO())
    {
        logger::info(shader.getInfoLog());
        logger::debug(shader.getInfoDebugLog());
        return false;
    }

    std::vector<u32> spvCode;
    glslang::GlslangToSpv(*program.getIntermediate(language), spvCode);

    glslang::FinalizeProcess();

    VkShaderModuleCreateInfo info{};
    info.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    info.codeSize = spvCode.size() * sizeof(u32);
    info.pCode = spvCode.data();

    VkResult res = vkCreateShaderModule(device, &info, nullptr, &module);
    if (res != VK_SUCCESS)
        return false;

    return true;
}

static inline VkPrimitiveTopology primitiveTypeToVk(PrimitiveType type)
{
    switch (type)
    {
    case PrimitiveType::POINT_LIST:
        return VK_PRIMITIVE_TOPOLOGY_POINT_LIST;
    case PrimitiveType::LINE_LIST:
        return VK_PRIMITIVE_TOPOLOGY_LINE_LIST;
    case PrimitiveType::LINE_STRIP:
        return VK_PRIMITIVE_TOPOLOGY_LINE_STRIP;
    case PrimitiveType::TRIANGEL_LIST:
        return VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    case PrimitiveType::TRIANGEL_STRIP:
        return VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP;
    }
}

PipelineBuilder::PipelineBuilder(const Device &device) : m_device(device)
{}

PipelineBuilder &PipelineBuilder::setVertexPath(const std::string &path)
{
    m_info.vertexPath = path;
    return *this;
}

PipelineBuilder &PipelineBuilder::setFragmentPath(const std::string &path)
{
    m_info.fragmentPath = path;
    return *this;
}

PipelineBuilder &PipelineBuilder::addDynamicState(VkDynamicState state)
{
    if (std::find(m_info.dynamicStates.begin(), m_info.dynamicStates.end(), state) != m_info.dynamicStates.end())
    {
        logger::debug("PipelineBuiler: ", state, " contains");
        return *this;
    }
    m_info.dynamicStates.emplace_back(state);
    return *this;
}

PipelineBuilder &PipelineBuilder::setPrimitiveType(PrimitiveType type)
{
    m_info.primitiveType = type;
    return *this;
}

ObjResult<Pipeline> PipelineBuilder::build() const
{
    VkShaderModule vertexShader;
    VkShaderModule fragmentShader;

    if (!compileShader(m_device, m_info.vertexPath, VK_SHADER_STAGE_VERTEX_BIT, vertexShader))
        return ObjResult<Pipeline>(error::makeError(error::PipelineError::FAILED_COMPILE_SHADER, VK_ERROR_INITIALIZATION_FAILED));

    if (!compileShader(m_device, m_info.fragmentPath, VK_SHADER_STAGE_FRAGMENT_BIT, fragmentShader))
        return ObjResult<Pipeline>(error::makeError(error::PipelineError::FAILED_COMPILE_SHADER, VK_ERROR_INITIALIZATION_FAILED));

    VkPipelineShaderStageCreateInfo vertexInfo{};
    vertexInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    vertexInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
    vertexInfo.module = vertexShader;
    vertexInfo.pName = "main";

    VkPipelineShaderStageCreateInfo fragmentInfo{};
    fragmentInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    fragmentInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
    fragmentInfo.module = fragmentShader;
    fragmentInfo.pName = "main";

    VkPipelineShaderStageCreateInfo shaderInfos[] = {vertexInfo, fragmentInfo};

    VkPipelineDynamicStateCreateInfo dynamicStateCreateInfo{};
    dynamicStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
    dynamicStateCreateInfo.dynamicStateCount = m_info.dynamicStates.size();
    dynamicStateCreateInfo.pDynamicStates = m_info.dynamicStates.data();

    VkVertexInputBindingDescription bindingDescriptions[1];
    bindingDescriptions[0].binding = 0;
    bindingDescriptions[0].stride = sizeof(Vertex);
    bindingDescriptions[0].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

    VkVertexInputAttributeDescription attributeDescriptions[4];
    attributeDescriptions[0].binding = 0;
    attributeDescriptions[0].location = 0;
    attributeDescriptions[0].format = VK_FORMAT_R32G32_SFLOAT;
    attributeDescriptions[0].offset = offsetof(Vertex, position);

    attributeDescriptions[1].binding = 0;
    attributeDescriptions[1].location = 1;
    attributeDescriptions[1].format = VK_FORMAT_R32G32B32A32_SFLOAT;
    attributeDescriptions[1].offset = offsetof(Vertex, color);

    attributeDescriptions[2].binding = 0;
    attributeDescriptions[2].location = 2;
    attributeDescriptions[2].format = VK_FORMAT_R32G32_SFLOAT;
    attributeDescriptions[2].offset = offsetof(Vertex, texCoord);

    attributeDescriptions[3].binding = 0;
    attributeDescriptions[3].location = 3;
    attributeDescriptions[3].format = VK_FORMAT_R32_SFLOAT;
    attributeDescriptions[3].offset = offsetof(Vertex, texId);

    VkPipelineVertexInputStateCreateInfo vertexInputStateCreateInfo{};
    vertexInputStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    vertexInputStateCreateInfo.vertexBindingDescriptionCount = 1;
    vertexInputStateCreateInfo.pVertexBindingDescriptions = bindingDescriptions;
    vertexInputStateCreateInfo.vertexAttributeDescriptionCount = 4;
    vertexInputStateCreateInfo.pVertexAttributeDescriptions = attributeDescriptions;

    VkPipelineInputAssemblyStateCreateInfo assemblyStateCreateInfo{};
    assemblyStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    assemblyStateCreateInfo.topology = primitiveTypeToVk(m_info.primitiveType);
    assemblyStateCreateInfo.primitiveRestartEnable = VK_FALSE;

    vkDestroyShaderModule(m_device, vertexShader, nullptr);
    vkDestroyShaderModule(m_device, fragmentShader, nullptr);

    return ObjResult<Pipeline>(VK_NULL_HANDLE, VK_NULL_HANDLE, &m_device);
}

} // namespace vk
