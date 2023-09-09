#pragma once

// https://vkguide.dev/docs/extra-chapter/abstracting_descriptors/
// TODO: look over how vkguide uses this, particularly with the pool per frame in flight and
// clearing the pool every frame; currently just plugged directly into my other stuff
// TODO: reconsider which hashmap works best for the cache
// TODO: implement this way of caching in other areas, e.g. pipeline layouts, render passes

#include <VLK/types.h>
#include <VLK/vulkan.h>

#include <_incl_global.hpp>

namespace VLK {

void InitDescriptors(Vulkan& vulkan);

// DESCRIPTOR POOLS / ALLOCATION

void DescAllocInit(DescAllocator& alloc, VkDevice device);
void DescAllocCleanup(DescAllocator& alloc);
VkDescriptorPool
DescAllocCreatePool(DescAllocator& alloc, uint32_t count, VkDescriptorPoolCreateFlags flags);
VkDescriptorPool DescAllocGetPool(DescAllocator& alloc);
void DescAllocResetPools(DescAllocator& alloc);
bool DescAllocAllocate(
	DescAllocator& alloc, VkDescriptorSet* set, VkDescriptorSetLayout layout
);

// DESCRIPTOR SET LAYOUTS

void DescCacheInit(DescLayoutCache& cache, VkDevice device);
void DescCacheCleanup(DescLayoutCache& cache);
VkDescriptorSetLayout
DescCacheCreateLayout(DescLayoutCache& cache, VkDescriptorSetLayoutCreateInfo* info);

// DESCRIPTOR BUILDER

DescBuilder DescBuilderCreate(DescLayoutCache* layoutCache, DescAllocator* allocator);
void DescBuilderBindBuffer(
	DescBuilder& builder, uint32_t binding, VkDescriptorBufferInfo* bufferInfo,
	VkDescriptorType type, VkShaderStageFlags stageFlags
);
void DescBuilderBindImage(
	DescBuilder& builder, uint32_t binding, VkDescriptorImageInfo* imageInfo,
	VkDescriptorType type, VkShaderStageFlags stageFlags
);
bool DescBuilderBuild(
	DescBuilder& builder, VkDescriptorSet& set, VkDescriptorSetLayout& layout
);
bool DescBuilderBuild(DescBuilder& builder, VkDescriptorSet& set);

// CLEANUP

void CleanupVkDescMain(void* vulkan, uint32_t i);
void CleanupVkDescFrame(void* vulkan, uint32_t i);

} // namespace VLK