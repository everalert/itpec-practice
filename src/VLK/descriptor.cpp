#include "descriptor.h"

#include <VLK/cleanup.h>
#include <VLK/types.h>
#include <VLK/vulkan.h>

void VLK::InitDescriptors(Vulkan& vulkan) {
	DescAllocInit(vulkan.descAlloc, vulkan.device);
	DescCacheInit(vulkan.descCache, vulkan.device);
	EnqueueCleanupFunc(vulkan.coreCleanupQueue, { (void*)&vulkan, 0, CleanupVkDescMain });
	vulkan.descAllocFrames.resize(vulkan.maxFramesInFlight);
	for (uint32_t i = 0; i < vulkan.maxFramesInFlight; ++i) {
		vulkan.descAllocFrames[i] = {};
		DescAllocInit(vulkan.descAllocFrames[i], vulkan.device);
		EnqueueCleanupFunc(vulkan.coreCleanupQueue, { (void*)&vulkan, i, CleanupVkDescFrame });
	}
}

// DESCRIPTOR POOLS / ALLOCATION

void VLK::DescAllocInit(DescAllocator& alloc, VkDevice device) { alloc.device = device; }

void VLK::DescAllocCleanup(DescAllocator& alloc) {
	for (auto pool : alloc.freePools)
		vkDestroyDescriptorPool(alloc.device, pool, nullptr);
	for (auto pool : alloc.usedPools)
		vkDestroyDescriptorPool(alloc.device, pool, nullptr);
}

VkDescriptorPool VLK::DescAllocCreatePool(
	DescAllocator& alloc, uint32_t count, VkDescriptorPoolCreateFlags flags
) {
	std::vector<VkDescriptorPoolSize> sizes;
	sizes.reserve(alloc.descSizes.size());
	for (auto sz : alloc.descSizes) {
		sizes.push_back({ sz.first, uint32_t(sz.second * count) });
	}
	VkDescriptorPoolCreateInfo poolInfo{};
	poolInfo.sType		   = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	poolInfo.flags		   = flags;
	poolInfo.maxSets	   = count;
	poolInfo.poolSizeCount = (uint32_t)sizes.size();
	poolInfo.pPoolSizes	   = sizes.data();

	VkDescriptorPool descriptorPool;
	if (vkCreateDescriptorPool(alloc.device, &poolInfo, nullptr, &descriptorPool) != VK_SUCCESS)
		throw std::runtime_error("DescAllocCreatePool: vkCreateDescriptorPool failed!");
	return descriptorPool;
}

VkDescriptorPool VLK::DescAllocGetPool(DescAllocator& alloc) {
	if (alloc.freePools.size() > 0) {
		VkDescriptorPool pool = alloc.freePools.back();
		alloc.freePools.pop_back();
		return pool;
	}
	return DescAllocCreatePool(alloc, 1024, 0);
}

bool VLK::DescAllocAllocate(
	DescAllocator& alloc, VkDescriptorSet* set, VkDescriptorSetLayout layout
) {
	if (alloc.currentPool == VK_NULL_HANDLE) {
		alloc.currentPool = DescAllocGetPool(alloc);
		alloc.usedPools.push_back(alloc.currentPool);
	}

	VkDescriptorSetAllocateInfo allocInfo{};
	allocInfo.sType				 = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	allocInfo.pNext				 = nullptr;
	allocInfo.pSetLayouts		 = &layout;
	allocInfo.descriptorPool	 = alloc.currentPool;
	allocInfo.descriptorSetCount = 1;

	VkResult allocResult = vkAllocateDescriptorSets(alloc.device, &allocInfo, set);
	bool needReallocate	 = false;
	switch (allocResult) {
	case VK_SUCCESS:
		return true;
	case VK_ERROR_FRAGMENTED_POOL:
	case VK_ERROR_OUT_OF_POOL_MEMORY:
		needReallocate = true;
		break;
	default:
		return false;
	}

	if (needReallocate) {
		alloc.currentPool = DescAllocGetPool(alloc);
		alloc.usedPools.push_back(alloc.currentPool);
		allocResult = vkAllocateDescriptorSets(alloc.device, &allocInfo, set);
		if (allocResult == VK_SUCCESS)
			return true;
	}
	throw std::runtime_error("DescAllocAllocate: vkAllocateDescriptorSets failed!");
	// return false;
}

void VLK::DescAllocResetPools(DescAllocator& alloc) {
	for (auto pool : alloc.usedPools) {
		vkResetDescriptorPool(alloc.device, pool, 0);
		alloc.freePools.push_back(pool);
	}
	alloc.usedPools.clear();
	alloc.currentPool = VK_NULL_HANDLE;
}

// DESCRIPTOR LAYOUTS

void VLK::DescCacheInit(DescLayoutCache& cache, VkDevice device) { cache.device = device; }

void VLK::DescCacheCleanup(DescLayoutCache& cache) {
	for (auto pair : cache.layoutCache)
		vkDestroyDescriptorSetLayout(cache.device, pair.second, nullptr);
}

VkDescriptorSetLayout
VLK::DescCacheCreateLayout(DescLayoutCache& cache, VkDescriptorSetLayoutCreateInfo* info) {
	DescLayoutInfo layoutInfo;
	layoutInfo.bindings.reserve(info->bindingCount);
	bool isSorted	= true;
	int lastBinding = -1;

	for (uint32_t i = 0; i < info->bindingCount; i++) {
		layoutInfo.bindings.push_back(info->pBindings[i]);
		if ((int)info->pBindings[i].binding > lastBinding)
			lastBinding = info->pBindings[i].binding;
		else
			isSorted = false;
	}
	if (!isSorted) {
		std::sort(
			layoutInfo.bindings.begin(), layoutInfo.bindings.end(),
			[](VkDescriptorSetLayoutBinding& a, VkDescriptorSetLayoutBinding& b) {
				return a.binding < b.binding;
			}
		);
	}

	auto it = cache.layoutCache.find(layoutInfo);
	if (it != cache.layoutCache.end())
		return (*it).second;

	VkDescriptorSetLayout layout;
	if (vkCreateDescriptorSetLayout(cache.device, info, nullptr, &layout) != VK_SUCCESS)
		throw std::runtime_error("DescCacheCreateLayout: vkCreateDescriptorSetLayout failed!");
	cache.layoutCache[layoutInfo] = layout;
	return layout;
}

size_t VLK::DescLayoutInfo::Hash() const {
	size_t result = std::hash<size_t>()(bindings.size());
	for (const VkDescriptorSetLayoutBinding& b : bindings) {
		size_t binding_hash
			= b.binding | b.descriptorType << 8 | b.descriptorCount << 16 | b.stageFlags << 24;
		result ^= std::hash<size_t>()(binding_hash);
	}
	return result;
}

bool VLK::DescLayoutInfo::operator==(const DescLayoutInfo& other) const {
	if (other.bindings.size() != bindings.size())
		return false;

	for (size_t i = 0; i < bindings.size(); i++) {
		if (other.bindings[i].binding != bindings[i].binding)
			return false;
		if (other.bindings[i].descriptorType != bindings[i].descriptorType)
			return false;
		if (other.bindings[i].descriptorCount != bindings[i].descriptorCount)
			return false;
		if (other.bindings[i].stageFlags != bindings[i].stageFlags)
			return false;
	}

	return true;
}

// DESCRIPTOR BUILDER

VLK::DescBuilder
VLK::DescBuilderCreate(DescLayoutCache* layoutCache, DescAllocator* allocator) {
	DescBuilder builder;
	builder.cache = layoutCache;
	builder.alloc = allocator;
	return builder;
}

void VLK::DescBuilderBindBuffer(
	DescBuilder& builder, uint32_t binding, VkDescriptorBufferInfo* bufferInfo,
	VkDescriptorType type, VkShaderStageFlags stageFlags
) {
	VkDescriptorSetLayoutBinding newBinding{};
	newBinding.descriptorCount	  = 1;
	newBinding.descriptorType	  = type;
	newBinding.pImmutableSamplers = nullptr;
	newBinding.stageFlags		  = stageFlags;
	newBinding.binding			  = binding;
	builder.bindings.push_back(newBinding);

	VkWriteDescriptorSet newWrite{};
	newWrite.sType			 = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	newWrite.pNext			 = nullptr;
	newWrite.descriptorCount = 1;
	newWrite.descriptorType	 = type;
	newWrite.pBufferInfo	 = bufferInfo;
	newWrite.dstBinding		 = binding;

	builder.writes.push_back(newWrite);
}

void VLK::DescBuilderBindImage(
	DescBuilder& builder, uint32_t binding, VkDescriptorImageInfo* imageInfo,
	VkDescriptorType type, VkShaderStageFlags stageFlags
) {
	VkDescriptorSetLayoutBinding newBinding{};
	newBinding.descriptorCount	  = 1;
	newBinding.descriptorType	  = type;
	newBinding.pImmutableSamplers = nullptr;
	newBinding.stageFlags		  = stageFlags;
	newBinding.binding			  = binding;
	builder.bindings.push_back(newBinding);

	VkWriteDescriptorSet newWrite{};
	newWrite.sType			 = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	newWrite.pNext			 = nullptr;
	newWrite.descriptorCount = 1;
	newWrite.descriptorType	 = type;
	newWrite.pImageInfo		 = imageInfo;
	newWrite.dstBinding		 = binding;

	builder.writes.push_back(newWrite);
}

bool VLK::DescBuilderBuild(
	DescBuilder& builder, VkDescriptorSet& set, VkDescriptorSetLayout& layout
) {
	VkDescriptorSetLayoutCreateInfo layoutInfo{};
	layoutInfo.sType		= VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	layoutInfo.pNext		= nullptr;
	layoutInfo.pBindings	= builder.bindings.data();
	layoutInfo.bindingCount = builder.bindings.size();

	layout = DescCacheCreateLayout(*builder.cache, &layoutInfo);
	bool success{ DescAllocAllocate(*builder.alloc, &set, layout) };
	if (!success)
		return false;

	for (VkWriteDescriptorSet& w : builder.writes)
		w.dstSet = set;

	vkUpdateDescriptorSets(
		builder.alloc->device, builder.writes.size(), builder.writes.data(), 0, nullptr
	);
	return true;
}

bool VLK::DescBuilderBuild(DescBuilder& builder, VkDescriptorSet& set) {
	VkDescriptorSetLayout layout;
	return DescBuilderBuild(builder, set, layout);
}

// CLEANUP

void VLK::CleanupVkDescMain(void* vulkan, uint32_t i) {
	Vulkan* v = static_cast<Vulkan*>(vulkan);
	DescCacheCleanup(v->descCache);
	DescAllocCleanup(v->descAlloc);
}

void VLK::CleanupVkDescFrame(void* vulkan, uint32_t i) {
	Vulkan* v = static_cast<Vulkan*>(vulkan);
	DescAllocCleanup(v->descAllocFrames[i]);
}