#pragma once

// TODO: stop RebuildSwapchain from halting the entire app on minimize

#include <VLK/vulkan.h>
#include <platform/renderer.h>

#include <_incl_global.hpp>

namespace VLK {

struct SwapchainSupportDetails {
	VkSurfaceCapabilitiesKHR capabilities;
	std::vector<VkSurfaceFormatKHR> formats;
	std::vector<VkPresentModeKHR> presentModes;
};

// SETUP & INITIALIZATION

void BuildSwapchain(Vulkan& vulkan);

void RebuildSwapchain(Vulkan& vulkan);

void InitRenderPass(Vulkan& vulkan);

// HELPERS

SwapchainSupportDetails QuerySwapchainSupport(VkPhysicalDevice, VkSurfaceKHR);

VkSurfaceFormatKHR SelectSurfaceFormat(const std::vector<VkSurfaceFormatKHR>&);

VkPresentModeKHR SelectPresentMode(const std::vector<VkPresentModeKHR>&);

VkExtent2D SelectExtent(Vulkan& vulkan, const VkSurfaceCapabilitiesKHR&);

// CLEANUP

void CleanupVkSwapchain(void* vulkan, uint32_t i);
void CleanupVkRenderPass(void* vulkan, uint32_t i);

} // namespace VLK