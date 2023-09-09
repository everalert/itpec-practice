#pragma once

#include <platform/renderer.h>

#include <_incl_global.hpp>

namespace VLK {

const std::vector<const char*> DeviceExtensions{
	VK_KHR_SWAPCHAIN_EXTENSION_NAME,
};

struct QueueFamilyIndices {
	std::optional<uint32_t> graphicsFamily;
	std::optional<uint32_t> presentFamily;
};

// MAIN FUNCTIONS

void SelectPhysicalDevice(Vulkan&);

void CreateLogicalDevice(Vulkan&);

// HELPERS

bool IsDeviceSuitable(VkPhysicalDevice, VkSurfaceKHR);

VkSampleCountFlagBits GetMaxUsableSampleCount(VkPhysicalDevice);

bool CheckDeviceExtensionSupport(VkPhysicalDevice, VkSurfaceKHR);

QueueFamilyIndices GetQueueFamilies(VkPhysicalDevice, VkSurfaceKHR);

bool IsQueueFamilyComplete(QueueFamilyIndices&);

} // namespace VLK