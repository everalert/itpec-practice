#include "imgui.h"

#include <VLK/cleanup.h>
#include <VLK/command.h>
#include <VLK/vulkan.h>
#include <platform/imgui.h>

void VLK::InitImgui(Vulkan& vulkan, std::unordered_map<std::string, PLAT::ImguiFont>& fonts) {
	VkDescriptorPoolSize descPoolSizes[]{ { VK_DESCRIPTOR_TYPE_SAMPLER, 1000 },
										  { VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1000 },
										  { VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 1000 },
										  { VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 1000 },
										  { VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, 1000 },
										  { VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER, 1000 },
										  { VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1000 },
										  { VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1000 },
										  { VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 1000 },
										  { VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, 1000 },
										  { VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, 1000 } };
	VkDescriptorPoolCreateInfo descPoolInfo{};
	descPoolInfo.sType		   = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	descPoolInfo.flags		   = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
	descPoolInfo.maxSets	   = 1000;
	descPoolInfo.poolSizeCount = std::size(descPoolSizes);
	descPoolInfo.pPoolSizes	   = descPoolSizes;

	if (vkCreateDescriptorPool(vulkan.device, &descPoolInfo, nullptr, &vulkan.descImgui)
		!= VK_SUCCESS) {
		std::runtime_error("Failed to create imgui descriptor pool!");
	}

	ImGui::CreateContext();

	ImGui_ImplGlfw_InitForVulkan(vulkan.windowDef->window, true);
	ImGui_ImplVulkan_InitInfo vulkanInit{};
	vulkanInit.Instance		  = vulkan.instance;
	vulkanInit.PhysicalDevice = vulkan.physicalDevice;
	vulkanInit.Device		  = vulkan.device;
	vulkanInit.Queue		  = vulkan.graphicsQueue;
	vulkanInit.DescriptorPool = vulkan.descImgui;
	vulkanInit.MinImageCount  = vulkan.maxFramesInFlight;
	vulkanInit.ImageCount	  = vulkan.maxFramesInFlight;
	vulkanInit.MSAASamples	  = vulkan.msaaSamples;

	ImGui_ImplVulkan_Init(&vulkanInit, vulkan.renderPass);

	PLAT::ImguiSetupFonts(fonts);

	SendImmediateCommand(vulkan, [&](VkCommandBuffer cmd) {
		ImGui_ImplVulkan_CreateFontsTexture(cmd);
	});

	ImGui_ImplVulkan_DestroyFontUploadObjects();

	EnqueueCleanupFunc(vulkan.coreCleanupQueue, { (void*)&vulkan, 0, CleanupImgui });
}

void VLK::CleanupImgui(void* vulkan, uint32_t i) {
	Vulkan* v = static_cast<Vulkan*>(vulkan);
	vkDestroyDescriptorPool(v->device, v->descImgui, nullptr);
	ImGui_ImplVulkan_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}