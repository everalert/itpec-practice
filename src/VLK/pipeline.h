#pragma once

#include <VLK/vulkan.h>

#include <_incl_global.hpp>

namespace VLK {

// FIXME: implement resource management similar to descriptor.h

void MakeGraphicsPipeline(VkDevice device, uint32_t, VkPipelineShaderStageCreateInfo*, VkExtent2D&, VkSampleCountFlagBits, VkRenderPass&, VkPipelineLayout&, VkPipeline&);

VkShaderModule CreateShaderModule(VkDevice device, std::vector<char>& code);

} // namespace VLK