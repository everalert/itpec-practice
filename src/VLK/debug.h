#pragma once

#include <_incl_global.hpp>

namespace VLK {

#ifdef NDEBUG
const bool EnableValidationLayers{ false };
const bool EnableShowAvailableExtensions{ false };
#else
const bool EnableValidationLayers{ true };
const bool EnableShowAvailableExtensions{ true };
#endif

const std::vector<const char*> ValidationLayers{
	"VK_LAYER_KHRONOS_validation",
};

void SetupDebugMessenger(VkInstance, VkDebugUtilsMessengerEXT&);

bool CheckValidationLayerSupport();

VKAPI_ATTR VkBool32 VKAPI_CALL
DebugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT, VkDebugUtilsMessageTypeFlagsEXT, const VkDebugUtilsMessengerCallbackDataEXT*, void*);

void PopulateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT&);

void ShowAvailableExtensions();

VkResult
CreateDebugUtilsMessengerEXT(VkInstance, const VkDebugUtilsMessengerCreateInfoEXT*, const VkAllocationCallbacks*, VkDebugUtilsMessengerEXT*);

void DestroyDebugUtilsMessengerEXT(VkInstance, VkDebugUtilsMessengerEXT, const VkAllocationCallbacks*);

} // namespace VLK