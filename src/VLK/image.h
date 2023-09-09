#pragma once

#include <VLK/command.h>
#include <VLK/memory.h>

#include <_incl_global.hpp>

namespace VLK {

// INITIALIZATION

void InitImageViews(Vulkan& vulkan);

void InitColorResources(Vulkan& vulkan);

void InitDepthResources(Vulkan& vulkan);

void InitFramebuffers(Vulkan& vulkan);

// RESOURCES

void CreateImage(uint32_t, uint32_t, uint32_t, VkSampleCountFlagBits, VkFormat, VkImageTiling, VkImageUsageFlags, VmaAllocator, VmaMemoryUsage, AllocatedImage&);
void CreateImage(VmaAllocator, VmaMemoryUsage, VkImageCreateInfo&, AllocatedImage&);

VkImageView CreateImageView(VkDevice, VkImage, VkFormat, VkImageAspectFlags, uint32_t);

void GenerateMipmaps(Vulkan& vulkan, VkImage, VkFormat, uint32_t, uint32_t, uint32_t);

// HELPERS

void CopyBufferToImage(Vulkan& vulkan, VkBuffer, VkImage, uint32_t, uint32_t);

void WriteImageViaStagingBuffer(Vulkan& vulkan, size_t, void*, VkImageCreateInfo&, AllocatedImage&);

void TransitionImageLayout(
	Vulkan& vulkan, VkImage, VkFormat, VkImageLayout, VkImageLayout, uint32_t
);

VkFormat FindSupportedFormat(
	VkPhysicalDevice, const std::vector<VkFormat>&, VkImageTiling, VkFormatFeatureFlags
);

VkFormat FindDepthFormat(VkPhysicalDevice);

bool HasStencilComponent(VkFormat);

// CLEANUP

void CleanupVkImageViewSwap(void* vulkan, uint32_t i);
void CleanupVkImageViewColor(void* vulkan, uint32_t i);
void CleanupVkImageViewDepth(void* vulkan, uint32_t i);

void CleanupVkImageColor(void* vulkan, uint32_t i);
void CleanupVkImageDepth(void* vulkan, uint32_t i);

void CleanupVkFramebuffer(void* vulkan, uint32_t i);

} // namespace VLK