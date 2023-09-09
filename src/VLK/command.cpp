#include "command.h"

#include <VLK/cleanup.h>
#include <VLK/device.h>
#include <VLK/initializers.hpp>
#include <VLK/memory.h>
#include <VLK/vulkan.h>

void VLK::InitCommands(Vulkan& vulkan) {
	// graphics pool
	CreateCommandPool(vulkan, vulkan.commandPool);
	EnqueueCleanupFunc(vulkan.coreCleanupQueue, { (void*)&vulkan, 0, CleanupVkCmdPool });
	vulkan.commandBuffer.resize(vulkan.maxFramesInFlight);
	CreateCommandBuffers(
		vulkan, vulkan.commandPool, vulkan.commandBuffer.size(), vulkan.commandBuffer.data()
	);
	// upload pool
	CreateCommandPool(vulkan, vulkan.uploadContext.commandPool);
	EnqueueCleanupFunc(vulkan.coreCleanupQueue, { (void*)&vulkan, 0, CleanupVkUpCtxCmdPool });
	CreateCommandBuffers(
		vulkan, vulkan.uploadContext.commandPool, 1, &vulkan.uploadContext.commandBuffer
	);
}

void VLK::CreateCommandPool(Vulkan& vulkan, VkCommandPool& pool) {
	QueueFamilyIndices qfi{ VLK::GetQueueFamilies(vulkan.physicalDevice, vulkan.surface) };
	auto createInfo{ VLK::MakeCommandPoolCI(
		qfi.graphicsFamily.value(), VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT
	) };
	if (vkCreateCommandPool(vulkan.device, &createInfo, nullptr, &pool) != VK_SUCCESS)
		throw std::runtime_error("Failed to create command pool!");
}

void VLK::CreateCommandBuffers(
	Vulkan& vulkan, VkCommandPool& pool, uint32_t bufferCount, VkCommandBuffer* buffer
) {
	auto allocInfo{ VLK::MakeCommandBufferAllocateInfo(pool, bufferCount) };
	if (vkAllocateCommandBuffers(vulkan.device, &allocInfo, buffer) != VK_SUCCESS)
		throw std::runtime_error("Failed to allocate command buffers!");
}

void VLK::SendRenderCommand(
	Vulkan& vulkan, VkCommandBuffer commandBuffer, uint32_t imageIndex
) {
	VkCommandBufferBeginInfo beginInfo{};
	beginInfo.sType			   = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	beginInfo.flags			   = 0;
	beginInfo.pInheritanceInfo = nullptr;

	if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS)
		throw std::runtime_error("Failed to begin recording command buffer!");

	VkRenderPassBeginInfo renderPassInfo{};
	renderPassInfo.sType			 = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
	renderPassInfo.renderPass		 = vulkan.renderPass;
	renderPassInfo.framebuffer		 = vulkan.swapChainFrameBuffers[imageIndex];
	renderPassInfo.renderArea.offset = { 0, 0 };
	renderPassInfo.renderArea.extent = vulkan.swapChainExtent;
	std::array<VkClearValue, 2> clearColor{}; // in same order as attachments
	clearColor[0].color			   = { { (float)16/255, (float)16/255, (float)16/255, 1.0f } };
	clearColor[1].depthStencil	   = { 1.0f, 0 };
	renderPassInfo.clearValueCount = static_cast<uint32_t>(clearColor.size());
	renderPassInfo.pClearValues	   = clearColor.data();

	vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

	VkViewport viewport{ VLK::MakeViewport(vulkan.swapChainExtent) };
	vkCmdSetViewport(commandBuffer, 0, 1, &viewport);
	VkRect2D scissor{ VLK::MakeScissor(vulkan.swapChainExtent) };
	vkCmdSetScissor(commandBuffer, 0, 1, &scissor);

	// // VLK::MaterialResource* prevMaterial{ nullptr };
	// VkPipeline prevPipeline{ 0 };
	// VLK::TextureResource* prevTexture{ nullptr };
	// VLK::ModelResource* prevModel{ nullptr };
	// for (VLK::RenderObject renderObject : vulkan.renderObjects) {
	// 	if (prevModel != renderObject.model) {
	// 		VkBuffer vertexBuffers[]{ renderObject.model->vertices.buffer };
	// 		VkDeviceSize offsets[]{ 0 };
	// 		vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers, offsets);
	// 		vkCmdBindIndexBuffer(
	// 			commandBuffer, renderObject.model->indices.buffer, 0, VK_INDEX_TYPE_UINT32
	// 		);
	// 		prevModel = renderObject.model;
	// 	}

	// 	if (prevPipeline != renderObject.pipeline) {

	// 		vkCmdBindPipeline(
	// 			commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, renderObject.pipeline
	// 		);
	// 		prevPipeline = renderObject.pipeline;
	// 	}

	// 	vkCmdBindDescriptorSets(
	// 		commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, renderObject.pipelineLayout, 0, 1,
	// 		&renderObject.descSets[vulkan.currentFrame], 0, nullptr
	// 	);

	// 	VLK::MeshPushConstants constants{
	// 		PreparePushConstants(vulkan.cameraMatrix, renderObject)
	// 	};
	// 	vkCmdPushConstants(
	// 		commandBuffer, renderObject.pipelineLayout, VK_SHADER_STAGE_VERTEX_BIT, 0,
	// 		sizeof(VLK::MeshPushConstants), &constants
	// 	);

	// 	vkCmdDrawIndexed(commandBuffer, renderObject.model->size, 1, 0, 0, 0);
	// }

	ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), commandBuffer);
	vkCmdEndRenderPass(commandBuffer);

	if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS)
		throw std::runtime_error("Failed to end recording command buffer!");
}

void VLK::SendImmediateCommand(
	VLK::Vulkan& vulkan, std::function<void(VkCommandBuffer cmd)>&& function
) {
	VkCommandBuffer buffer{ vulkan.uploadContext.commandBuffer };
	auto beginInfo{ VLK::MakeCommandBufferBI(VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT) };

	if (vkBeginCommandBuffer(buffer, &beginInfo) != VK_SUCCESS)
		throw std::runtime_error("Failed to begin sending command buffer!");
	function(buffer);
	if (vkEndCommandBuffer(buffer) != VK_SUCCESS)
		throw std::runtime_error("Failed to end sending command buffer!");

	VkSubmitInfo submit{ VLK::MakeSubmitInfo(&buffer) };
	vkQueueSubmit(vulkan.graphicsQueue, 1, &submit, vulkan.uploadContext.fence);

	vkWaitForFences(vulkan.device, 1, &vulkan.uploadContext.fence, true, UINT64_MAX);
	vkResetFences(vulkan.device, 1, &vulkan.uploadContext.fence);
	vkResetCommandPool(vulkan.device, vulkan.uploadContext.commandPool, 0);
}

void VLK::CreateSyncObjects(Vulkan& vulkan) {
	vulkan.imageAvailable.resize(vulkan.maxFramesInFlight);
	vulkan.renderFinished.resize(vulkan.maxFramesInFlight);
	vulkan.inFlight.resize(vulkan.maxFramesInFlight);
	auto semaInfoImg{ VLK::MakeSemaphoreCI() };
	auto semaInfoRen{ VLK::MakeSemaphoreCI() };
	auto fenceInfoIF{ VLK::MakeFenceCI(VK_FENCE_CREATE_SIGNALED_BIT) };
	for (uint32_t i = 0; i < vulkan.maxFramesInFlight; ++i) {
		if (vkCreateSemaphore(vulkan.device, &semaInfoImg, nullptr, &vulkan.imageAvailable[i])
				!= VK_SUCCESS
			|| vkCreateSemaphore(
				   vulkan.device, &semaInfoRen, nullptr, &vulkan.renderFinished[i]
			   ) != VK_SUCCESS
			|| vkCreateFence(vulkan.device, &fenceInfoIF, nullptr, &vulkan.inFlight[i])
				   != VK_SUCCESS)
			throw std::runtime_error("Failed to create semaphores!");

		EnqueueCleanupFunc(vulkan.coreCleanupQueue, { (void*)&vulkan, i, CleanupVkFrameSync });
	}

	auto fenceInfoUL{ VLK::MakeFenceCI() };
	if (vkCreateFence(vulkan.device, &fenceInfoUL, nullptr, &vulkan.uploadContext.fence)
		!= VK_SUCCESS)
		throw std::runtime_error("Failed to create upload context fence!");

	EnqueueCleanupFunc(vulkan.coreCleanupQueue, { (void*)&vulkan, 0, CleanupVkUpCtxFence });
}

void VLK::CreateUniformBuffers(Vulkan& vulkan, VLK::RenderObject& renderObject) {
	VkDeviceSize bufferSize{ sizeof(VLK::UniformBufferObject) };

	renderObject.uniformBuffers.resize(vulkan.maxFramesInFlight);
	renderObject.uniformBuffersMapped.resize(vulkan.maxFramesInFlight);

	for (size_t i = 0; i < vulkan.maxFramesInFlight; ++i) {
		CreateBuffer(
			vulkan, bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VMA_MEMORY_USAGE_CPU_TO_GPU,
			renderObject.uniformBuffers[i]
		);
		vmaMapMemory(
			vulkan.allocator, renderObject.uniformBuffers[i].allocation,
			&renderObject.uniformBuffersMapped[i]
		);
	}
}

void VLK::UpdateUniformBuffer(Vulkan& vulkan, uint32_t currentImage) {
	// VLK::UniformBufferObject ubo{};
	// ...
	// memcpy(m_uniformBuffersVMAMapped[currentImage], &ubo, sizeof(ubo));
}

VLK::MeshPushConstants
VLK::PreparePushConstants(glm::mat4& cameraMatrix, VLK::RenderObject& renderObject) {
	// static auto startTime{ std::chrono::high_resolution_clock::now() };
	// auto currentTime{ std::chrono::high_resolution_clock::now() };
	// float time{
	// 	std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime)
	// 		.count()
	// };

	glm::mat4 translation = glm::translate(glm::mat4{ 1.0 }, renderObject.position);
	glm::mat4 model{ renderObject.baseTransform * renderObject.rotation * translation };

	VLK::MeshPushConstants push{};
	push.renderMatrix = cameraMatrix * model;
	return push;
}

// CLEANUP

void VLK::CleanupVkCmdPool(void* vulkan, uint32_t i) {
	Vulkan* v = static_cast<Vulkan*>(vulkan);
	vkDestroyCommandPool(v->device, v->commandPool, nullptr);
}
void VLK::CleanupVkUpCtxCmdPool(void* vulkan, uint32_t i) {
	Vulkan* v = static_cast<Vulkan*>(vulkan);
	vkDestroyCommandPool(v->device, v->uploadContext.commandPool, nullptr);
}
void VLK::CleanupVkFrameSync(void* vulkan, uint32_t i) {
	Vulkan* v = static_cast<Vulkan*>(vulkan);
	vkDestroySemaphore(v->device, v->imageAvailable[i], nullptr);
	vkDestroySemaphore(v->device, v->renderFinished[i], nullptr);
	vkDestroyFence(v->device, v->inFlight[i], nullptr);
}
void VLK::CleanupVkUpCtxFence(void* vulkan, uint32_t i) {
	Vulkan* v = static_cast<Vulkan*>(vulkan);
	vkDestroyFence(v->device, v->uploadContext.fence, nullptr);
}