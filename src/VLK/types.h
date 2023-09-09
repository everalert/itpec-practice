#pragma once

// FIXME: this is getting out of hand/very quickly becoming not just about types, need to
// rethink where all this stuff goes while avoiding circular includes

#include <_incl_global.hpp>

namespace VLK {

// OBJECT BUFFERS

struct Vertex {
	glm::vec3 pos;
	glm::vec3 color;
	glm::vec2 texCoord;
	bool operator==(const Vertex& other) const;
};
VkVertexInputBindingDescription GetVertexBindingDescription();
std::array<VkVertexInputAttributeDescription, 3> GetVertexAttributeDescriptions();

struct UniformBufferObject {
	alignas(16) glm::mat4 model;
	alignas(16) glm::mat4 view;
	alignas(16) glm::mat4 projection;
};

struct MeshPushConstants {
	glm::vec4 data;
	glm::mat4 renderMatrix;
};

// MEMORY

struct AllocatedBuffer {
	VkBuffer buffer;
	VmaAllocation allocation;
};

struct AllocatedImage {
	VkImage image;
	VmaAllocation allocation;
};

// DESCRIPTORS

struct DescAllocator {
	VkDevice device{ VK_NULL_HANDLE };
	VkDescriptorPool currentPool{ VK_NULL_HANDLE };
	std::vector<VkDescriptorPool> freePools;
	std::vector<VkDescriptorPool> usedPools;
	std::vector<std::pair<VkDescriptorType, float>> descSizes
		= { { VK_DESCRIPTOR_TYPE_SAMPLER, 0.5f },
			{ VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 4.f },
			{ VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 4.f },
			{ VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 1.f },
			{ VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, 1.f },
			{ VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER, 1.f },
			{ VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 2.f },
			{ VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 2.f },
			{ VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 1.f },
			{ VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, 1.f },
			{ VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, 0.5f } };
};

struct DescLayoutInfo {
	std::vector<VkDescriptorSetLayoutBinding> bindings;
	bool operator==(const DescLayoutInfo& other) const;
	size_t Hash() const;
};

struct DescLayoutHash {
	std::size_t operator()(const DescLayoutInfo& k) const { return k.Hash(); }
};

struct DescLayoutCache {
	VkDevice device{ VK_NULL_HANDLE };
	std::unordered_map<DescLayoutInfo, VkDescriptorSetLayout, DescLayoutHash> layoutCache;
};

struct DescBuilder {
	std::vector<VkWriteDescriptorSet> writes;
	std::vector<VkDescriptorSetLayoutBinding> bindings;
	DescLayoutCache* cache;
	DescAllocator* alloc;
};

// RESOURCES

struct ShaderDesc {
	std::string path;
	VkShaderStageFlagBits stage;
};

struct TextureResource {
	uint32_t mipLevels;
	AllocatedImage image;
	VkImageView view;
	VkSampler sampler;
};

struct ModelResource {
	AllocatedBuffer vertices;
	AllocatedBuffer indices;
	uint32_t size;
};

struct RenderObject {
	std::vector<AllocatedBuffer> uniformBuffers;
	std::vector<void*> uniformBuffersMapped;
	ModelResource* model{ nullptr };
	TextureResource* texture{ nullptr };
	VkPipeline pipeline;
	VkPipelineLayout pipelineLayout;
	VkDescriptorSetLayout descSetLayout;
	std::vector<VkDescriptorSet> descSets;
	glm::mat4 baseTransform;
	glm::vec3 position;
	glm::mat4 rotation;
};

// UNORGANIZED

struct UploadContext {
	VkFence fence;
	VkCommandPool commandPool;
	VkCommandBuffer commandBuffer;
};

} // namespace VLK

namespace std {
template <> struct hash<VLK::Vertex> {
	size_t operator()(VLK::Vertex const& vertex) const {
		return ((hash<glm::vec3>()(vertex.pos) ^ (hash<glm::vec3>()(vertex.color) << 1)) >> 1)
			 ^ (hash<glm::vec2>()(vertex.texCoord) << 1);
	}
};
} // namespace std