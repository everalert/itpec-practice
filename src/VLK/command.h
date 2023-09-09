#pragma once

// FIXME: does uniform buffer and push constant updating go here?

#include <VLK/types.h>
#include <VLK/vulkan.h>

#include <_incl_global.hpp>

namespace VLK {

// NOTE: UploadContext struct was here but temp removed to solve circular include

void InitCommands(Vulkan& vulkan);

void CreateCommandPool(Vulkan& vulkan, VkCommandPool&);

void CreateCommandBuffers(Vulkan& vulkan, VkCommandPool&, uint32_t, VkCommandBuffer*);

void SendRenderCommand(Vulkan& vulkan, VkCommandBuffer buffer, uint32_t index);

void SendImmediateCommand(Vulkan& vulkan, std::function<void(VkCommandBuffer cmd)>&& function);

void CreateSyncObjects(Vulkan& vulkan);

void CreateUniformBuffers(Vulkan& vulkan, RenderObject&);

void UpdateUniformBuffer(Vulkan& vulkan, uint32_t);

MeshPushConstants
PreparePushConstants(glm::mat4& cameraMatrix, RenderObject& renderObject);

// CLEANUP

void CleanupVkCmdPool(void* vulkan, uint32_t i);
void CleanupVkUpCtxCmdPool(void* vulkan, uint32_t i);
void CleanupVkFrameSync(void* vulkan, uint32_t i);
void CleanupVkUpCtxFence(void* vulkan, uint32_t i);

} // namespace VLK