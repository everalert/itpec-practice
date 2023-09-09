#include "renderer.h"

#include "VLK/cleanup.h"
#include "VLK/debug.h"
#include "VLK/descriptor.h"
#include "VLK/device.h"
#include "VLK/image.h"
#include "VLK/imgui.h"
#include "VLK/initializers.hpp"
#include "VLK/instance.h"
#include "VLK/memory.h"
#include "VLK/pipeline.h"
#include "VLK/swapchain.h"
#include "utility.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>
#define TINYOBJLOADER_IMPLEMENTATION
#include <tinyobjloader/tiny_obj_loader.h>

// SETUP & INITIALIZATION

void PLAT::InitRenderer(Renderer& renderer, WindowDef& windowDef) {
	VLK::Vulkan& vulkan{ renderer.backend };
	// backend setup
	vulkan.windowDef = &windowDef;
	auto reqExt{ PLAT::GetRequiredVulkanWindowExtensions(VLK::EnableValidationLayers) };
	VLK::CreateInstance(reqExt, vulkan.instance);
	VLK::SetupDebugMessenger(vulkan.instance, vulkan.debugMessenger);
	PLAT::CreateVulkanWindowSurface(windowDef, vulkan.instance, nullptr, &vulkan.surface);
	VLK::SelectPhysicalDevice(vulkan);
	VLK::CreateLogicalDevice(vulkan);
	VLK::InitAllocator(vulkan);
	// renderer initialization
	VLK::InitCommands(vulkan);
	VLK::CreateSyncObjects(vulkan);
	VLK::InitRenderPass(vulkan);
	VLK::InitDescriptors(vulkan);
	VLK::InitImgui(vulkan, renderer.fonts);
	// swapchain
	VLK::BuildSwapchain(vulkan);
	VLK::InitImageViews(vulkan);
	VLK::InitColorResources(vulkan);
	VLK::InitDepthResources(vulkan);
	VLK::InitFramebuffers(vulkan);
	// scene-related stuff (objects, textures, etc.) handled outside of init
}

void PLAT::CleanupRenderer(Renderer& renderer) {
	VLK::Vulkan& vulkan{ renderer.backend };
	RendererWaitIdle(renderer);

	// VLK::FlushCleanupQueue(vulkan.resourceCleanupQueue);

	VLK::FlushCleanupQueue(vulkan.swapchainCleanupQueue);

	VLK::FlushCleanupQueue(vulkan.coreCleanupQueue);

	vmaDestroyAllocator(vulkan.allocator);
	vkDestroyDevice(vulkan.device, nullptr);
	if (VLK::EnableValidationLayers)
		VLK::DestroyDebugUtilsMessengerEXT(vulkan.instance, vulkan.debugMessenger, nullptr);
	vkDestroySurfaceKHR(vulkan.instance, vulkan.surface, nullptr);
	vkDestroyInstance(vulkan.instance, nullptr);
}

void PLAT::RenderFrame(Renderer& renderer) {
	VLK::Vulkan& vulkan{ renderer.backend };
	vkWaitForFences(
		vulkan.device, 1, &vulkan.inFlight[vulkan.currentFrame], VK_TRUE, UINT64_MAX
	);

	uint32_t imageIndex;
	VkResult result{ vkAcquireNextImageKHR(
		vulkan.device, vulkan.swapChain, UINT64_MAX, vulkan.imageAvailable[vulkan.currentFrame],
		VK_NULL_HANDLE, &imageIndex
	) };
	if (result == VK_ERROR_OUT_OF_DATE_KHR) {
		VLK::RebuildSwapchain(vulkan);
		return;
	} else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
		throw std::runtime_error("Failed to acquire swapchain image!");

	VLK::UpdateUniformBuffer(vulkan, vulkan.currentFrame);
	vkResetFences(vulkan.device, 1, &vulkan.inFlight[vulkan.currentFrame]);
	vkResetCommandBuffer(vulkan.commandBuffer[vulkan.currentFrame], 0);
	VLK::SendRenderCommand(vulkan, vulkan.commandBuffer[vulkan.currentFrame], imageIndex);

	VkSubmitInfo submitInfo{ VLK::MakeSubmitInfo(&vulkan.commandBuffer[vulkan.currentFrame]) };
	VkSemaphore waitSemaphores[]{ vulkan.imageAvailable[vulkan.currentFrame] };
	VkSemaphore signalSemaphores[]{ vulkan.renderFinished[vulkan.currentFrame] };
	VkPipelineStageFlags waitStages[]{ VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
	submitInfo.waitSemaphoreCount	= 1;
	submitInfo.pWaitSemaphores		= waitSemaphores;
	submitInfo.pWaitDstStageMask	= waitStages;
	submitInfo.signalSemaphoreCount = 1;
	submitInfo.pSignalSemaphores	= signalSemaphores;

	if (vkQueueSubmit(
			vulkan.graphicsQueue, 1, &submitInfo, vulkan.inFlight[vulkan.currentFrame]
		)
		!= VK_SUCCESS)
		throw std::runtime_error("Failed to submit draw command buffer!");

	VkPresentInfoKHR presentInfo{};
	presentInfo.sType			   = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
	presentInfo.waitSemaphoreCount = 1;
	presentInfo.pWaitSemaphores	   = signalSemaphores;
	VkSwapchainKHR swapChains[]{ vulkan.swapChain };
	presentInfo.swapchainCount = 1;
	presentInfo.pSwapchains	   = swapChains;
	presentInfo.pImageIndices  = &imageIndex;
	presentInfo.pResults	   = nullptr;

	result = vkQueuePresentKHR(vulkan.presentQueue, &presentInfo);

	if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR
		|| vulkan.windowDef->isFramebufferResized) {
		vulkan.windowDef->isFramebufferResized = false;
		VLK::RebuildSwapchain(vulkan);
	} else if (result != VK_SUCCESS)
		throw std::runtime_error("Failed to present swap chain image!");

	vulkan.currentFrame = (vulkan.currentFrame + 1) % vulkan.maxFramesInFlight;
}

// RESOURCE INTERFACE

void PLAT::CreateModel(
	Renderer& renderer, const std::string& identifier, std::vector<VLK::Vertex>& vertices,
	std::vector<uint32_t>& indices
) {
	VLK::Vulkan& vulkan{ renderer.backend };
	if (vulkan.models.find(identifier) != vulkan.models.end())
		throw std::runtime_error("CreateModel: '" + identifier + "' already exists.");

	VLK::ModelResource model{};
	model.size = static_cast<uint32_t>(indices.size());

	// NOTE: should i just use one buffer for both with an offset for indices?
	VLK::WriteBufferViaStagingBuffer(
		vulkan, vertices.size() * sizeof(VLK::Vertex), vertices.data(),
		VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, model.vertices
	);
	VLK::WriteBufferViaStagingBuffer(
		vulkan, indices.size() * sizeof(uint32_t), indices.data(),
		VK_BUFFER_USAGE_INDEX_BUFFER_BIT, model.indices
	);

	vulkan.models[identifier] = model;
}

void PLAT::CreatePlane(
	const glm::vec2& size, const glm::vec3& color, std::vector<VLK::Vertex>& vertices,
	std::vector<uint32_t>& indices
) {
	glm::vec3 s{ size, 0.0f };
	vertices.resize(4);
	vertices[0] = { glm::vec3(-0.5f, -0.5f, 0.0f) * s, color, glm::vec2(0.0f, 0.0f) };
	vertices[1] = { glm::vec3(-0.5f, 0.5f, 0.0f) * s, color, glm::vec2(0.0f, 1.0f) };
	vertices[2] = { glm::vec3(0.5f, 0.5f, 0.0f) * s, color, glm::vec2(1.0f, 1.0f) };
	vertices[3] = { glm::vec3(0.5f, -0.5f, 0.0f) * s, color, glm::vec2(1.0f, 0.0f) };
	indices.resize(6);
	indices = { 0, 2, 1, 3, 2, 0 };
}

void PLAT::CreateTilePlane(
	const std::vector<bool>& tilemap, uint32_t tilesW, uint32_t tilesH,
	const glm::vec2& tileSize, const glm::vec3& color, std::vector<VLK::Vertex>& vertices,
	std::vector<uint32_t>& indices
) {
	assert(
		tilemap.size() == tilesW * tilesH
		&& "CreateTilePlane: Tile map size does not match given dimensions."
	);

	glm::vec3 s{ tileSize, 0.0f };
	glm::vec3 start{ -tileSize.x * (float)tilesW * 0.5f, -tileSize.y * (float)tilesH * 0.5f,
					 0.0f };
	int64_t tileCount{ std::count_if(tilemap.begin(), tilemap.end(), [](bool b) {
		return b;
	}) };

	uint32_t vtxI{ 0 }, idxI{ 0 }, row, col;
	glm::vec2 pos;
	vertices.resize(tileCount * 4);
	indices.resize(tileCount * 6);
	for (size_t i = 0; i < tilemap.size(); ++i) {
		if (!tilemap[i])
			continue;
		row = i / tilesW;
		col = i % tilesW;

		vertices[vtxI + 0]
			= { glm::vec3(row + 0, col + 0, 0) * s + start, color, glm::vec2(0, 0) };
		vertices[vtxI + 1]
			= { glm::vec3(row + 0, col + 1, 0) * s + start, color, glm::vec2(0, 1) };
		vertices[vtxI + 2]
			= { glm::vec3(row + 1, col + 1, 0) * s + start, color, glm::vec2(1, 1) };
		vertices[vtxI + 3]
			= { glm::vec3(row + 1, col + 0, 0) * s + start, color, glm::vec2(1, 0) };

		indices[idxI + 0] = vtxI + 0;
		indices[idxI + 1] = vtxI + 2;
		indices[idxI + 2] = vtxI + 1;
		indices[idxI + 3] = vtxI + 3;
		indices[idxI + 4] = vtxI + 2;
		indices[idxI + 5] = vtxI + 0;

		vtxI += 4;
		idxI += 6;
	}
}

// TODO: make nicer for side texture application; move color/uv to ubo?
void PLAT::CreateCube(
	const glm::vec3& size, const glm::vec3& color, std::vector<VLK::Vertex>& vertices,
	std::vector<uint32_t>& indices
) {
	vertices.resize(8);
	vertices[0] = { glm::vec3(-0.5f, -0.5f, 0.5f) * size, color, glm::vec2(0.0f, 0.0f) };
	vertices[1] = { glm::vec3(-0.5f, 0.5f, 0.5f) * size, color, glm::vec2(0.0f, 1.0f) };
	vertices[2] = { glm::vec3(0.5f, 0.5f, 0.5f) * size, color, glm::vec2(1.0f, 1.0f) };
	vertices[3] = { glm::vec3(0.5f, -0.5f, 0.5f) * size, color, glm::vec2(1.0f, 0.0f) };
	vertices[4] = { glm::vec3(-0.5f, -0.5f, -0.5f) * size, color, glm::vec2(0.0f, 1.0f) };
	vertices[5] = { glm::vec3(-0.5f, 0.5f, -0.5f) * size, color, glm::vec2(0.0f, 0.0f) };
	vertices[6] = { glm::vec3(0.5f, 0.5f, -0.5f) * size, color, glm::vec2(1.0f, 0.0f) };
	vertices[7] = { glm::vec3(0.5f, -0.5f, -0.5f) * size, color, glm::vec2(1.0f, 1.0f) };
	indices.resize(36);
	indices = { 0, 2, 1, 3, 2, 0, 4, 3, 0, 7, 3, 4, 7, 2, 3, 6, 2, 7,
				6, 1, 2, 5, 1, 6, 5, 0, 1, 4, 0, 5, 7, 4, 6, 6, 4, 5 };
}

void PLAT::LoadOBJ(
	const std::string& path, std::vector<VLK::Vertex>& vertices, std::vector<uint32_t>& indices
) {
	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;
	std::string warn, err;

	if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, path.c_str()))
		throw std::runtime_error(warn + err);

	std::unordered_map<VLK::Vertex, uint32_t> uniqueVertices{};

	for (const auto& shape : shapes) {
		for (const auto& index : shape.mesh.indices) {
			VLK::Vertex vertex{};

			vertex.pos = {
				attrib.vertices[3 * index.vertex_index + 0],
				attrib.vertices[3 * index.vertex_index + 1],
				attrib.vertices[3 * index.vertex_index + 2],
			};
			vertex.texCoord = {
				attrib.texcoords[2 * index.texcoord_index + 0],
				1.0f - attrib.texcoords[2 * index.texcoord_index + 1],
			};
			vertex.color = { 1.0f, 1.0f, 1.0f };

			if (uniqueVertices.count(vertex) == 0) {
				uniqueVertices[vertex] = static_cast<uint32_t>(vertices.size());
				vertices.push_back(vertex);
			}
			indices.push_back(uniqueVertices[vertex]);
		}
	}
}

void PLAT::LoadModel(
	Renderer& renderer, const std::string& identifier, const std::string& path
) {
	std::vector<VLK::Vertex> vertices;
	std::vector<uint32_t> indices;
	LoadOBJ(path, vertices, indices);
	CreateModel(renderer, identifier, vertices, indices);
}

void PLAT::DeleteModel(Renderer& renderer, const std::string& identifier) {
	VLK::Vulkan& vulkan{ renderer.backend };
	if (vulkan.models.find(identifier) == vulkan.models.end())
		throw std::runtime_error("DeleteModel: '" + identifier + "' does not exist.");

	vmaDestroyBuffer(
		vulkan.allocator, vulkan.models[identifier].vertices.buffer,
		vulkan.models[identifier].vertices.allocation
	);
	vmaDestroyBuffer(
		vulkan.allocator, vulkan.models[identifier].indices.buffer,
		vulkan.models[identifier].indices.allocation
	);

	vulkan.models.erase(identifier);
}

VLK::ModelResource* PLAT::GetModel(Renderer& renderer, const std::string& identifier) {
	VLK::Vulkan& vulkan{ renderer.backend };
	auto it{ vulkan.models.find(identifier) };
	if (it == vulkan.models.end())
		return nullptr;
	return &(*it).second;
}

void PLAT::LoadTexture(
	Renderer& renderer, const std::string& identifier, const std::string& path
) {
	VLK::Vulkan& vulkan{ renderer.backend };
	if (vulkan.textures.find(identifier) != vulkan.textures.end())
		throw std::runtime_error("LoadTexture: '" + identifier + "' already exists.");

	VLK::TextureResource texture{};

	int texWidth, texHeight, texChannels;
	stbi_uc* pixels{
		stbi_load(path.c_str(), &texWidth, &texHeight, &texChannels, STBI_rgb_alpha)
	};
	VkDeviceSize imageSize{ static_cast<VkDeviceSize>(texWidth * texHeight * 4) };
	texture.mipLevels
		= static_cast<uint32_t>(std::floor(std::log2(std::max(texWidth, texHeight)))) + 1;

	if (!pixels)
		throw std::runtime_error("Failed to load texture image!");

	auto imageInfo{ VLK::MakeImageCI(
		texWidth, texHeight, texture.mipLevels, VK_SAMPLE_COUNT_1_BIT, VK_FORMAT_R8G8B8A8_SRGB,
		VK_IMAGE_TILING_OPTIMAL,
		VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT
			| VK_IMAGE_USAGE_SAMPLED_BIT
	) };
	VLK::WriteImageViaStagingBuffer(
		vulkan, static_cast<size_t>(imageSize), pixels, imageInfo, texture.image
	);
	VLK::GenerateMipmaps(
		vulkan, texture.image.image, VK_FORMAT_R8G8B8A8_SRGB, texWidth, texHeight,
		texture.mipLevels
	);
	texture.view = VLK::CreateImageView(
		vulkan.device, texture.image.image, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_ASPECT_COLOR_BIT,
		texture.mipLevels
	);

	VkPhysicalDeviceProperties properties{};
	vkGetPhysicalDeviceProperties(vulkan.physicalDevice, &properties);
	VkSamplerCreateInfo samplerInfo{};
	samplerInfo.sType					= VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
	samplerInfo.magFilter				= VK_FILTER_LINEAR;
	samplerInfo.minFilter				= VK_FILTER_LINEAR;
	samplerInfo.addressModeU			= VK_SAMPLER_ADDRESS_MODE_REPEAT;
	samplerInfo.addressModeV			= VK_SAMPLER_ADDRESS_MODE_REPEAT;
	samplerInfo.addressModeW			= VK_SAMPLER_ADDRESS_MODE_REPEAT;
	samplerInfo.anisotropyEnable		= VK_TRUE;
	samplerInfo.maxAnisotropy			= properties.limits.maxSamplerAnisotropy;
	samplerInfo.borderColor				= VK_BORDER_COLOR_INT_OPAQUE_BLACK;
	samplerInfo.unnormalizedCoordinates = VK_FALSE;
	samplerInfo.compareEnable			= VK_FALSE;
	samplerInfo.compareOp				= VK_COMPARE_OP_ALWAYS;
	samplerInfo.mipmapMode				= VK_SAMPLER_MIPMAP_MODE_LINEAR;
	samplerInfo.mipLodBias				= 0.0f;
	samplerInfo.minLod					= 0.0f;
	samplerInfo.maxLod					= static_cast<float>(texture.mipLevels);

	if (vkCreateSampler(vulkan.device, &samplerInfo, nullptr, &texture.sampler) != VK_SUCCESS)
		throw std::runtime_error("Failed to create texture sampler!");

	vulkan.textures[identifier] = texture;
}

void PLAT::DeleteTexture(Renderer& renderer, const std::string& identifier) {

	VLK::Vulkan& vulkan{ renderer.backend };
	if (vulkan.textures.find(identifier) == vulkan.textures.end())
		throw std::runtime_error("DeleteTexture: '" + identifier + "' does not exist.");

	vkDestroySampler(vulkan.device, vulkan.textures[identifier].sampler, nullptr);
	vkDestroyImageView(vulkan.device, vulkan.textures[identifier].view, nullptr);
	vmaDestroyImage(
		vulkan.allocator, vulkan.textures[identifier].image.image,
		vulkan.textures[identifier].image.allocation
	);

	vulkan.textures.erase(identifier);
}

VLK::TextureResource* PLAT::GetTexture(Renderer& renderer, const std::string& identifier) {

	VLK::Vulkan& vulkan{ renderer.backend };
	auto it{ vulkan.textures.find(identifier) };
	if (it == vulkan.textures.end())
		return nullptr;
	return &(*it).second;
}

VLK::RenderObject PLAT::CreateRenderObject(
	Renderer& renderer, const std::string& modelIdentifier,
	const std::string& textureIdentifier, const glm::mat4& transform,
	std::vector<VLK::ShaderDesc>& shaders, VkPushConstantRange& pushConst
) {
	VLK::Vulkan& vulkan{ renderer.backend };
	VLK::RenderObject renderObject;
	renderObject.model		   = GetModel(renderer, modelIdentifier);
	renderObject.texture	   = GetTexture(renderer, textureIdentifier);
	renderObject.baseTransform = transform;
	renderObject.position	   = glm::vec3(0.0f);
	renderObject.rotation	   = glm::mat4(1.0f);
	VLK::CreateUniformBuffers(vulkan, renderObject);

	VLK::DescBuilder descBuilder;
	renderObject.descSets.resize(vulkan.maxFramesInFlight);
	for (uint32_t i = 0; i < vulkan.maxFramesInFlight; ++i) {
		descBuilder = VLK::DescBuilderCreate(&vulkan.descCache, &vulkan.descAlloc);

		VkDescriptorBufferInfo bufferInfo{};
		bufferInfo.buffer = renderObject.uniformBuffers[i].buffer;
		bufferInfo.offset = 0;
		bufferInfo.range  = sizeof(VLK::UniformBufferObject);
		VLK::DescBuilderBindBuffer(
			descBuilder, 0, &bufferInfo, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
			VK_SHADER_STAGE_VERTEX_BIT
		);

		VkDescriptorImageInfo imageInfo{};
		imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		imageInfo.imageView	  = renderObject.texture->view;
		imageInfo.sampler	  = renderObject.texture->sampler;
		VLK::DescBuilderBindImage(
			descBuilder, 1, &imageInfo, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
			VK_SHADER_STAGE_FRAGMENT_BIT
		);

		VLK::DescBuilderBuild(
			descBuilder, renderObject.descSets[i], renderObject.descSetLayout
		);
	}

	std::vector<VkPipelineShaderStageCreateInfo> stages;
	for (uint32_t i = 0; i < shaders.size(); ++i) {
		auto code{ PLAT::ReadFile(shaders[i].path) };
		VkShaderModule shader{ VLK::CreateShaderModule(vulkan.device, code) };
		stages.push_back(VLK::MakePipelineShaderStageCI(shaders[i].stage, shader));
	}

	// NOTE: descSetLayout should be the same regardless of how many builds write it above
	auto layoutInfo{ VLK::MakePipelineLayoutCI(1, &renderObject.descSetLayout, 1, &pushConst) };
	if (vkCreatePipelineLayout(
			vulkan.device, &layoutInfo, nullptr, &renderObject.pipelineLayout
		)
		!= VK_SUCCESS)
		throw std::runtime_error("Failed to create pipeline layout!");

	VLK::MakeGraphicsPipeline(
		vulkan.device, stages.size(), stages.data(), vulkan.swapChainExtent, vulkan.msaaSamples,
		vulkan.renderPass, renderObject.pipelineLayout, renderObject.pipeline
	);

	// DONE

	for (auto stage : stages)
		vkDestroyShaderModule(vulkan.device, stage.module, nullptr);

	return renderObject;
}

void PLAT::CleanupRenderObject(Renderer& renderer, VLK::RenderObject& renderObject) {
	VLK::Vulkan& vulkan{ renderer.backend };
	for (size_t i = 0; i < vulkan.maxFramesInFlight; ++i) {
		vmaUnmapMemory(vulkan.allocator, renderObject.uniformBuffers[i].allocation);
		vmaDestroyBuffer(
			vulkan.allocator, renderObject.uniformBuffers[i].buffer,
			renderObject.uniformBuffers[i].allocation
		);
	}
	vkDestroyPipeline(vulkan.device, renderObject.pipeline, nullptr);
	vkDestroyPipelineLayout(vulkan.device, renderObject.pipelineLayout, nullptr);
}

// UTILITY

void PLAT::RendererWaitIdle(Renderer& renderer) {
	VLK::Vulkan& vulkan{ renderer.backend };
	vkDeviceWaitIdle(vulkan.device);
}

void PLAT::RendererUpdateCamera(Renderer& renderer, glm::mat4& matrix) {
	renderer.backend.cameraMatrix = matrix;
}