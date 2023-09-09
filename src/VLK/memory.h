#pragma once

#include <VLK/types.h>
#include <VLK/vulkan.h>

#include <_incl_global.hpp>

namespace VLK {

// NOTE: AllocatedBuffer and AllocatedImage used to be defined here, but moved to types.h due to
// circular dependency

void InitAllocator(Vulkan& vulkan);

void CreateBuffer(Vulkan& vulkan, VkDeviceSize, VkBufferUsageFlags, VmaMemoryUsage, AllocatedBuffer&);

void CopyBuffer(Vulkan& vulkan, VkBuffer, VkBuffer, VkDeviceSize);

void WriteBufferViaStagingBuffer(Vulkan& vulkan, size_t, void*, VkBufferUsageFlagBits, AllocatedBuffer&);

uint32_t FindMemoryType(
	VkPhysicalDevice physicalDevice, uint32_t typeFilter, VkMemoryPropertyFlags properties
);

} // namespace VLK