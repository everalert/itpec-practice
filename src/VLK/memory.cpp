#include "memory.h"

#include <VLK/cleanup.h>
#include <VLK/command.h>
#include <VLK/types.h>
#include <VLK/vulkan.h>
#include <platform/renderer.h>

// leaving this here because the VMA gods told me to leave it in one cpp file
#define VMA_IMPLEMENTATION
#include <vma/vk_mem_alloc.h>

void VLK::InitAllocator(Vulkan& vulkan) {
	VmaAllocatorCreateInfo allocatorInfo{};
	allocatorInfo.physicalDevice = vulkan.physicalDevice;
	allocatorInfo.device = vulkan.device;
	allocatorInfo.instance = vulkan.instance;
	vmaCreateAllocator(&allocatorInfo, &vulkan.allocator);
}

// TODO: check if the old tutorial createBuffer had anything useful to carry over
void VLK::CreateBuffer(
	Vulkan& vulkan, VkDeviceSize size, VkBufferUsageFlags usage, VmaMemoryUsage allocUsage,
	AllocatedBuffer& alloc
) {
	VmaAllocationCreateInfo vmaallocInfo{};
	vmaallocInfo.usage = allocUsage;

	VkBufferCreateInfo bufferInfo{};
	bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	bufferInfo.pNext = nullptr;
	bufferInfo.size = size;
	bufferInfo.usage = usage;

	if (vmaCreateBuffer(
			vulkan.allocator, &bufferInfo, &vmaallocInfo, &alloc.buffer, &alloc.allocation,
			nullptr
		)
		!= VK_SUCCESS)
		throw std::runtime_error("CreateBuffer: Failed to create VMA buffer!");
}

void VLK::CopyBuffer(Vulkan& vulkan, VkBuffer src, VkBuffer dst, VkDeviceSize size) {
	SendImmediateCommand(vulkan, [&](VkCommandBuffer cmd) {
		VkBufferCopy copyRegion{};
		copyRegion.size = size;
		vkCmdCopyBuffer(cmd, src, dst, 1, &copyRegion);
	});
}

void VLK::WriteBufferViaStagingBuffer(
	Vulkan& vulkan, size_t size, void* data, VkBufferUsageFlagBits usage,
	AllocatedBuffer& buffer
) {
	AllocatedBuffer stagingBuffer;
	CreateBuffer(
		vulkan, static_cast<uint32_t>(size), VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
		VMA_MEMORY_USAGE_CPU_ONLY, stagingBuffer
	);
	void* stagingData;
	vmaMapMemory(vulkan.allocator, stagingBuffer.allocation, &stagingData);
	memcpy(stagingData, data, size);
	vmaUnmapMemory(vulkan.allocator, stagingBuffer.allocation);
	CreateBuffer(
		vulkan, static_cast<uint32_t>(size), usage | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
		VMA_MEMORY_USAGE_GPU_ONLY, buffer
	);
	SendImmediateCommand(vulkan, [=](VkCommandBuffer cmd) {
		VkBufferCopy copy{};
		copy.dstOffset = 0;
		copy.srcOffset = 0;
		copy.size = size;
		vkCmdCopyBuffer(cmd, stagingBuffer.buffer, buffer.buffer, 1, &copy);
	});
	vmaDestroyBuffer(vulkan.allocator, stagingBuffer.buffer, stagingBuffer.allocation);
}

uint32_t VLK::FindMemoryType(
	VkPhysicalDevice physicalDevice, uint32_t typeFilter, VkMemoryPropertyFlags properties
) {
	VkPhysicalDeviceMemoryProperties memProperties;
	vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memProperties);

	for (uint32_t i = 0; i < memProperties.memoryTypeCount; ++i) {
		if (typeFilter & (1 << i)
			&& (memProperties.memoryTypes[i].propertyFlags & properties) == properties)
			return i;
	}

	throw std::runtime_error("Failed to find suitable memory type!");
}