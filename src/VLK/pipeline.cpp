#include "pipeline.h"

#include <VLK/initializers.hpp>

void VLK::MakeGraphicsPipeline(VkDevice device, 
	uint32_t shaderCount, VkPipelineShaderStageCreateInfo* shaders, VkExtent2D& extent,
	VkSampleCountFlagBits msaaSamples, VkRenderPass& renderPass,
	VkPipelineLayout& pipelineLayout, VkPipeline& graphicsPipeline
) {
	// FIXED-FUNCTION STATE

	// NOTE: these will be required at draw time instead of baked in now
	std::vector<VkDynamicState> states{ VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR };
	auto dynamicState{
		MakePipelineDynamicStateCI((uint32_t)states.size(), states.data())
	};

	auto bindDesc{ GetVertexBindingDescription() };
	auto attrDesc{ GetVertexAttributeDescriptions() };
	auto vertexInput{ MakePipelineVertexInputStateCI(
		1, &bindDesc, (uint32_t)attrDesc.size(), attrDesc.data()
	) };

	auto inputAssembly{
		MakePipelineInputAssemblyStateCI(VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST)
	};

	VkViewport viewport{ MakeViewport(extent) };
	VkRect2D scissor{ MakeScissor(extent) };
	VkPipelineViewportStateCreateInfo viewportState{
		MakePipelineViewportStateCI(1, &viewport, 1, &scissor)
	};

	auto rasterizer{ MakePipelineRasterizationStateCI(VK_POLYGON_MODE_FILL) };

	auto multisampling{ MakePipelineMultisamplingStateCI(msaaSamples) };

	auto stencil{ MakePipelineDepthStencilStateCI(0.0f, 1.0f) };

	auto colorBlendAttachment{ MakePipelineColorBlendAttachmentState() };
	auto colorBlending{ MakePipelineColorBlendStateCI(1, &colorBlendAttachment) };

	// NOW WE WIN

	VkGraphicsPipelineCreateInfo pipelineInfo{};
	pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
	pipelineInfo.stageCount = shaderCount;
	pipelineInfo.pStages = shaders;
	pipelineInfo.pVertexInputState = &vertexInput;
	pipelineInfo.pInputAssemblyState = &inputAssembly;
	pipelineInfo.pViewportState = &viewportState;
	pipelineInfo.pRasterizationState = &rasterizer;
	pipelineInfo.pMultisampleState = &multisampling;
	pipelineInfo.pDepthStencilState = &stencil;
	pipelineInfo.pColorBlendState = &colorBlending;
	pipelineInfo.pDynamicState = &dynamicState;
	pipelineInfo.layout = pipelineLayout;
	pipelineInfo.renderPass = renderPass;
	pipelineInfo.subpass = 0;
	pipelineInfo.basePipelineHandle
		= VK_NULL_HANDLE;				 // if you want to derive from another pipeline
	pipelineInfo.basePipelineIndex = -1; // alternative to via handle

	if (vkCreateGraphicsPipelines(
			device, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &graphicsPipeline
		)
		!= VK_SUCCESS)
		throw std::runtime_error("Failed to create graphics pipeline!");
}

VkShaderModule VLK::CreateShaderModule(VkDevice device, std::vector<char>& code) {
	// auto code{ PLAT::ReadFile(path) };

	VkShaderModuleCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	createInfo.codeSize = code.size();
	createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

	VkShaderModule shaderModule;
	if (vkCreateShaderModule(device, &createInfo, nullptr, &shaderModule) != VK_SUCCESS)
		throw std::runtime_error("Failed to create shader module!");

	return shaderModule;
}