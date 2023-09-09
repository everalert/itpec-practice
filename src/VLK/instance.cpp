#include "instance.h"

#include <VLK/debug.h>
#include <VLK/cleanup.h>

// TODO: fix naming/version stuff for future use
void VLK::CreateInstance(std::vector<const char*>& requiredExtensions, VkInstance& instance) {
	if (VLK::EnableValidationLayers && !VLK::CheckValidationLayerSupport())
		throw std::runtime_error("Validation layers requested, but not available.");

	VkApplicationInfo appInfo{};
	appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	appInfo.pApplicationName = "Hello Triangle";
	appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.pEngineName = "No Engine";
	appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.apiVersion = VK_API_VERSION_1_3;

	VkInstanceCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	createInfo.pApplicationInfo = &appInfo;
	createInfo.enabledExtensionCount = static_cast<uint32_t>(requiredExtensions.size());
	createInfo.ppEnabledExtensionNames = requiredExtensions.data();
	if (VLK::EnableValidationLayers) {
		VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{};
		createInfo.enabledLayerCount = static_cast<uint32_t>(VLK::ValidationLayers.size());
		createInfo.ppEnabledLayerNames = VLK::ValidationLayers.data();
		VLK::PopulateDebugMessengerCreateInfo(debugCreateInfo);
		createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*)&debugCreateInfo;
	} else {
		createInfo.enabledLayerCount = 0;
		createInfo.pNext = nullptr;
	}
	VLK::ShowAvailableExtensions();

	// TODO: check if all of the extensions returned by
	//		PLAT::GetRequiredVulkanWindowExtensions are included in the supported
	//		extensions list.

	if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS)
		throw std::runtime_error("Failed to create instance!");
}