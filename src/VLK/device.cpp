#include "device.h"

#include <VLK/debug.h>
#include <VLK/cleanup.h>
#include <VLK/swapchain.h>
#include <VLK/vulkan.h>
#include <platform/renderer.h>

// CORE FUNCTIONS

void VLK::SelectPhysicalDevice(Vulkan& vulkan) {
	uint32_t deviceCount{ 0 };
	vkEnumeratePhysicalDevices(vulkan.instance, &deviceCount, nullptr);

	if (deviceCount == 0)
		throw std::runtime_error("Failed to find GPUs with Vulkan support!");

	std::vector<VkPhysicalDevice> devices(deviceCount);
	vkEnumeratePhysicalDevices(vulkan.instance, &deviceCount, devices.data());

	for (const auto& device : devices) {
		if (IsDeviceSuitable(device, vulkan.surface)) {
			vulkan.physicalDevice = device;
			vulkan.msaaSamples = GetMaxUsableSampleCount(device);
			break;
		}
	}

	if (vulkan.physicalDevice == VK_NULL_HANDLE)
		throw std::runtime_error("Failed to find a suitable GPU!");
}

void VLK::CreateLogicalDevice(Vulkan& vulkan) {
	VLK::QueueFamilyIndices familyIndices{
		GetQueueFamilies(vulkan.physicalDevice, vulkan.surface)
	};

	std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
	std::set<uint32_t> uniqueQueueFamilies{ familyIndices.graphicsFamily.value(),
											familyIndices.presentFamily.value() };

	float queuePriority{ 1.0f };
	for (uint32_t queueFamily : uniqueQueueFamilies) {
		VkDeviceQueueCreateInfo queueCreateInfo{};
		queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		queueCreateInfo.queueFamilyIndex = queueFamily;
		queueCreateInfo.queueCount = 1;
		queueCreateInfo.pQueuePriorities = &queuePriority;
		queueCreateInfos.push_back(queueCreateInfo);
	}

	VkPhysicalDeviceFeatures deviceFeatures{};
	deviceFeatures.samplerAnisotropy = VK_TRUE;
	deviceFeatures.sampleRateShading = VK_TRUE; // antialias textures themselves too

	VkDeviceCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	createInfo.pQueueCreateInfos = queueCreateInfos.data();
	createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
	createInfo.pEnabledFeatures = &deviceFeatures;
	createInfo.enabledExtensionCount = static_cast<uint32_t>(VLK::DeviceExtensions.size());
	createInfo.ppEnabledExtensionNames = VLK::DeviceExtensions.data();
	if (VLK::EnableValidationLayers) {
		// for backward compatibility
		createInfo.enabledLayerCount = static_cast<uint32_t>(VLK::ValidationLayers.size());
		createInfo.ppEnabledLayerNames = VLK::ValidationLayers.data();
	} else
		createInfo.enabledLayerCount = 0;

	if (vkCreateDevice(vulkan.physicalDevice, &createInfo, nullptr, &vulkan.device)
		!= VK_SUCCESS)
		throw std::runtime_error("Failed to create logical device!");

	vkGetDeviceQueue(
		vulkan.device, familyIndices.graphicsFamily.value(), 0, &vulkan.graphicsQueue
	);
	vkGetDeviceQueue(
		vulkan.device, familyIndices.presentFamily.value(), 0, &vulkan.presentQueue
	);
}

// HELPERS

VLK::QueueFamilyIndices VLK::GetQueueFamilies(VkPhysicalDevice device, VkSurfaceKHR surface) {
	VLK::QueueFamilyIndices familyIndices;

	uint32_t queueFamilyCount{ 0 };
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);
	std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

	uint32_t i{ 0 };
	for (const auto& queueFamily : queueFamilies) {
		VkBool32 presentSupport{ false };
		vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &presentSupport);
		if (presentSupport)
			familyIndices.presentFamily = i;
		if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
			familyIndices.graphicsFamily = i;
		if (VLK::IsQueueFamilyComplete(familyIndices))
			break;
		++i;
	}

	return familyIndices;
}

bool VLK::IsQueueFamilyComplete(QueueFamilyIndices& qfi) {
	return qfi.graphicsFamily.has_value() && qfi.presentFamily.has_value();
}

// TODO: pick a device based on some kind of scoring or selection process
// rather than first in best dressed
bool VLK::IsDeviceSuitable(VkPhysicalDevice device, VkSurfaceKHR surface) {
	VLK::QueueFamilyIndices familyIndices = GetQueueFamilies(device, surface);

	VkPhysicalDeviceProperties deviceProperties;
	VkPhysicalDeviceFeatures deviceFeatures;
	vkGetPhysicalDeviceProperties(device, &deviceProperties);
	vkGetPhysicalDeviceFeatures(device, &deviceFeatures);

	bool extensionsSupported{ CheckDeviceExtensionSupport(device, surface) };

	bool swapChainAdequate{ false };
	if (extensionsSupported) {
		SwapchainSupportDetails swapChainSupport{ QuerySwapchainSupport(device, surface) };
		swapChainAdequate
			= !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
	}

	return VLK::IsQueueFamilyComplete(familyIndices) && extensionsSupported && swapChainAdequate
		&& deviceFeatures.samplerAnisotropy;
}

VkSampleCountFlagBits VLK::GetMaxUsableSampleCount(VkPhysicalDevice device) {
	VkPhysicalDeviceProperties physicalDeviceProperties;
	vkGetPhysicalDeviceProperties(device, &physicalDeviceProperties);

	VkSampleCountFlags counts = physicalDeviceProperties.limits.framebufferColorSampleCounts
							  & physicalDeviceProperties.limits.framebufferDepthSampleCounts;
	if (counts & VK_SAMPLE_COUNT_64_BIT)
		return VK_SAMPLE_COUNT_64_BIT;
	if (counts & VK_SAMPLE_COUNT_32_BIT)
		return VK_SAMPLE_COUNT_32_BIT;
	if (counts & VK_SAMPLE_COUNT_16_BIT)
		return VK_SAMPLE_COUNT_16_BIT;
	if (counts & VK_SAMPLE_COUNT_8_BIT)
		return VK_SAMPLE_COUNT_8_BIT;
	if (counts & VK_SAMPLE_COUNT_4_BIT)
		return VK_SAMPLE_COUNT_4_BIT;
	if (counts & VK_SAMPLE_COUNT_2_BIT)
		return VK_SAMPLE_COUNT_2_BIT;

	return VK_SAMPLE_COUNT_1_BIT;
}

bool VLK::CheckDeviceExtensionSupport(VkPhysicalDevice device, VkSurfaceKHR surface) {
	uint32_t extensionCount{ 0 };
	vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);
	std::vector<VkExtensionProperties> availableExtensions{ extensionCount };
	vkEnumerateDeviceExtensionProperties(
		device, nullptr, &extensionCount, availableExtensions.data()
	);

	std::set<std::string> requiredExtensions(
		VLK::DeviceExtensions.begin(), VLK::DeviceExtensions.end()
	);
	for (const auto& extension : availableExtensions)
		requiredExtensions.erase(extension.extensionName);

	return requiredExtensions.empty();
}