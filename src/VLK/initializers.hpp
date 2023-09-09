#pragma once

#include "types.h"

#include <_incl_global.hpp>

namespace VLK {

// VIEWPORT

inline VkViewport MakeViewport(VkExtent2D& extent) {
	VkViewport viewport{};
	viewport.x = 0.0f;
	viewport.y = 0.0f;
	viewport.width = (float)extent.width;
	viewport.height = (float)extent.height;
	viewport.minDepth = 0.0f;
	viewport.maxDepth = 1.0f;
	return viewport;
}

inline VkRect2D MakeScissor(VkExtent2D& extent) {
	VkRect2D scissor{};
	scissor.offset = { 0, 0 };
	scissor.extent = extent;
	return scissor;
}

// GRAPHICS PIPELINE

inline VkPipelineShaderStageCreateInfo
MakePipelineShaderStageCI(VkShaderStageFlagBits stage, VkShaderModule module) {
	VkPipelineShaderStageCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	createInfo.stage = stage;
	createInfo.module = module;
	createInfo.pName = "main"; // could change this for different shader entry point
	createInfo.pNext = nullptr;
	return createInfo;
}

inline VkPipelineDynamicStateCreateInfo
MakePipelineDynamicStateCI(uint32_t count, VkDynamicState* states) {
	VkPipelineDynamicStateCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
	createInfo.dynamicStateCount = count;
	createInfo.pDynamicStates = states;
	createInfo.pNext = nullptr;
	return createInfo;
}

inline VkPipelineVertexInputStateCreateInfo MakePipelineVertexInputStateCI(
	uint32_t bindCount, VkVertexInputBindingDescription* pBindDesc, uint32_t attrCount,
	VkVertexInputAttributeDescription* pAttrDesc
) {
	VkPipelineVertexInputStateCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
	createInfo.vertexBindingDescriptionCount = bindCount;
	createInfo.pVertexBindingDescriptions = pBindDesc;
	createInfo.vertexAttributeDescriptionCount = attrCount;
	createInfo.pVertexAttributeDescriptions = pAttrDesc;
	createInfo.pNext = nullptr;
	return createInfo;
}

inline VkPipelineInputAssemblyStateCreateInfo
MakePipelineInputAssemblyStateCI(VkPrimitiveTopology topology) {
	VkPipelineInputAssemblyStateCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
	createInfo.topology = topology;
	createInfo.primitiveRestartEnable = VK_FALSE;
	createInfo.pNext = nullptr;
	return createInfo;
}

inline VkPipelineViewportStateCreateInfo MakePipelineViewportStateCI(
	uint32_t viewportCount, VkViewport* pViewport, uint32_t scissorCount, VkRect2D* pScissor
) {
	VkPipelineViewportStateCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
	createInfo.viewportCount = viewportCount;
	createInfo.pViewports = pViewport;
	createInfo.scissorCount = scissorCount;
	createInfo.pScissors = pScissor;
	createInfo.pNext = nullptr;
	return createInfo;
}

inline VkPipelineRasterizationStateCreateInfo
MakePipelineRasterizationStateCI(VkPolygonMode polygonMode) {
	VkPipelineRasterizationStateCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
	createInfo.depthClampEnable = VK_FALSE;
	createInfo.rasterizerDiscardEnable = VK_FALSE;
	createInfo.polygonMode = polygonMode;
	createInfo.lineWidth = 1.0f;
	createInfo.cullMode = VK_CULL_MODE_BACK_BIT;
	createInfo.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
	createInfo.depthBiasEnable = VK_FALSE;
	createInfo.depthBiasConstantFactor = 0.0f;
	createInfo.depthBiasClamp = 0.0f;
	createInfo.depthBiasSlopeFactor = 0.0f;
	createInfo.pNext = nullptr;
	return createInfo;
}

inline VkPipelineMultisampleStateCreateInfo
MakePipelineMultisamplingStateCI(VkSampleCountFlagBits msaaSamples) {
	VkPipelineMultisampleStateCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
	createInfo.sampleShadingEnable = VK_TRUE;
	createInfo.rasterizationSamples = msaaSamples;
	createInfo.minSampleShading = 0.2f; // 1.0 = smooth
	createInfo.pSampleMask = nullptr;
	createInfo.alphaToCoverageEnable = VK_FALSE;
	createInfo.alphaToOneEnable = VK_FALSE;
	createInfo.pNext = nullptr;
	return createInfo;
}

inline VkPipelineDepthStencilStateCreateInfo
MakePipelineDepthStencilStateCI(float minDepth, float maxDepth) {
	VkPipelineDepthStencilStateCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
	createInfo.depthTestEnable = VK_TRUE;
	createInfo.depthWriteEnable = VK_TRUE;
	createInfo.depthCompareOp = VK_COMPARE_OP_LESS;
	createInfo.depthBoundsTestEnable = VK_FALSE;
	createInfo.minDepthBounds = minDepth;
	createInfo.maxDepthBounds = maxDepth;
	createInfo.stencilTestEnable = VK_FALSE;
	createInfo.front = {};
	createInfo.back = {};
	createInfo.pNext = nullptr;
	return createInfo;
}

inline VkPipelineColorBlendAttachmentState MakePipelineColorBlendAttachmentState() {
	VkPipelineColorBlendAttachmentState state{};
	state.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT
						 | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
	state.blendEnable = VK_FALSE;
	state.srcColorBlendFactor = VK_BLEND_FACTOR_ONE;
	state.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO;
	state.colorBlendOp = VK_BLEND_OP_ADD;
	state.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
	state.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
	state.alphaBlendOp = VK_BLEND_OP_ADD;
	return state;
}

inline VkPipelineColorBlendStateCreateInfo MakePipelineColorBlendStateCI(
	uint32_t attachStateCount, VkPipelineColorBlendAttachmentState* pAttachStates
) {
	VkPipelineColorBlendStateCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
	createInfo.logicOpEnable = VK_FALSE;
	createInfo.logicOp = VK_LOGIC_OP_COPY;
	createInfo.attachmentCount = attachStateCount;
	createInfo.pAttachments = pAttachStates;
	createInfo.blendConstants[0] = 0.0f;
	createInfo.blendConstants[1] = 0.0f;
	createInfo.blendConstants[2] = 0.0f;
	createInfo.blendConstants[3] = 0.0f;
	createInfo.pNext = nullptr;
	return createInfo;
}

inline VkPipelineLayoutCreateInfo MakePipelineLayoutCI(
	uint32_t layoutCount, VkDescriptorSetLayout* pLayouts, uint32_t pushRangeCount,
	VkPushConstantRange* pPushRanges
) {
	VkPipelineLayoutCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	createInfo.setLayoutCount = layoutCount;
	createInfo.pSetLayouts = pLayouts;
	createInfo.pushConstantRangeCount = pushRangeCount;
	createInfo.pPushConstantRanges = pPushRanges;
	createInfo.pNext = nullptr;
	return createInfo;
}

inline VkPushConstantRange MakePushConstRange(
	uint32_t size, VkShaderStageFlags flags
) {
	VkPushConstantRange pushConst{};
	pushConst.offset = 0;
	pushConst.size = size;
	pushConst.stageFlags = flags;
	return pushConst;
}

// SYNCHRONIZATION

inline VkFenceCreateInfo MakeFenceCI(VkFenceCreateFlags flags = 0) {
	VkFenceCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
	createInfo.pNext = nullptr;
	createInfo.flags = flags;
	return createInfo;
}

inline VkSemaphoreCreateInfo MakeSemaphoreCI(VkSemaphoreCreateFlags flags = 0) {
	VkSemaphoreCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
	createInfo.pNext = nullptr;
	createInfo.flags = flags;
	return createInfo;
}

// COMMAND BUFFERS

inline VkCommandPoolCreateInfo
MakeCommandPoolCI(uint32_t queueFamilyIndex, VkCommandPoolCreateFlags flags = 0) {
	VkCommandPoolCreateInfo info{};
	info.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	info.pNext = nullptr;
	info.flags = flags;
	info.queueFamilyIndex = queueFamilyIndex;
	return info;
}

inline VkCommandBufferBeginInfo MakeCommandBufferBI(VkCommandBufferUsageFlags flags = 0) {
	VkCommandBufferBeginInfo info{};
	info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	info.pNext = nullptr;
	info.pInheritanceInfo = nullptr;
	info.flags = flags;
	return info;
}

inline VkCommandBufferAllocateInfo MakeCommandBufferAllocateInfo(
	VkCommandPool pool, uint32_t count = 1,
	VkCommandBufferLevel level = VK_COMMAND_BUFFER_LEVEL_PRIMARY
) {
	VkCommandBufferAllocateInfo info = {};
	info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	info.pNext = nullptr;
	info.commandPool = pool;
	info.commandBufferCount = count;
	info.level = level;
	return info;
}

inline VkSubmitInfo MakeSubmitInfo(VkCommandBuffer* cmd) {
	VkSubmitInfo info{};
	info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	info.pNext = nullptr;
	info.waitSemaphoreCount = 0;
	info.pWaitSemaphores = nullptr;
	info.pWaitDstStageMask = nullptr;
	info.commandBufferCount = 1;
	info.pCommandBuffers = cmd;
	info.signalSemaphoreCount = 0;
	info.pSignalSemaphores = nullptr;
	return info;
}

// IMAGES

inline VkImageCreateInfo MakeImageCI(
	uint32_t width, uint32_t height, uint32_t mipLevels, VkSampleCountFlagBits numSamples,
	VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage
) {
	VkImageCreateInfo imageInfo{};
	imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
	imageInfo.imageType = VK_IMAGE_TYPE_2D;
	imageInfo.extent.width = width;
	imageInfo.extent.height = height;
	imageInfo.extent.depth = 1;
	imageInfo.mipLevels = mipLevels;
	imageInfo.arrayLayers = 1;
	imageInfo.format = format;
	imageInfo.tiling = tiling; // must be linear if you want to directly access pixels in buffer
	imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	imageInfo.usage = usage;
	imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
	imageInfo.samples = numSamples;
	imageInfo.flags = 0;
	return imageInfo;
}

} // namespace VLK