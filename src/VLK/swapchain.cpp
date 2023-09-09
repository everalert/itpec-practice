#include "swapchain.h"

#include <VLK/cleanup.h>
#include <VLK/device.h>
#include <VLK/image.h>
#include <VLK/vulkan.h>

void VLK::BuildSwapchain(Vulkan& vulkan) {
	SwapchainSupportDetails swapchainSupport{
		QuerySwapchainSupport(vulkan.physicalDevice, vulkan.surface)
	};

	VkSurfaceFormatKHR surfaceFormat{ SelectSurfaceFormat(swapchainSupport.formats) };
	VkPresentModeKHR presentMode{ SelectPresentMode(swapchainSupport.presentModes) };
	VkExtent2D extent{ SelectExtent(vulkan, swapchainSupport.capabilities) };

	uint32_t imageCount{ swapchainSupport.capabilities.minImageCount + 1 };
	if (swapchainSupport.capabilities.maxImageCount > 0
		&& imageCount > swapchainSupport.capabilities.maxImageCount)
		imageCount = swapchainSupport.capabilities.maxImageCount;

	VkSwapchainCreateInfoKHR createInfo{};
	createInfo.sType			= VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	createInfo.surface			= vulkan.surface;
	createInfo.minImageCount	= imageCount;
	createInfo.imageFormat		= surfaceFormat.format;
	createInfo.imageColorSpace	= surfaceFormat.colorSpace;
	createInfo.imageExtent		= extent;
	createInfo.imageArrayLayers = 1; // NOTE: change for stereoscopic 3D
	createInfo.imageUsage		= VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
	QueueFamilyIndices familyIndices{ GetQueueFamilies(vulkan.physicalDevice, vulkan.surface) };
	if (familyIndices.graphicsFamily != familyIndices.presentFamily) {
		uint32_t queueFamilyIndices[]{ familyIndices.graphicsFamily.value(),
									   familyIndices.presentFamily.value() };
		createInfo.imageSharingMode		 = VK_SHARING_MODE_CONCURRENT;
		createInfo.queueFamilyIndexCount = 2;
		createInfo.pQueueFamilyIndices	 = queueFamilyIndices;
	} else {
		createInfo.imageSharingMode		 = VK_SHARING_MODE_EXCLUSIVE;
		createInfo.queueFamilyIndexCount = 0;
		createInfo.pQueueFamilyIndices	 = nullptr;
	}
	createInfo.preTransform	  = swapchainSupport.capabilities.currentTransform;
	createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR; // NOTE: probably change this
																   // for transparent window?
	createInfo.presentMode	  = presentMode;
	createInfo.clipped		  = VK_TRUE;
	createInfo.oldSwapchain	  = VK_NULL_HANDLE; // NOTE: used when resizing window, etc.

	if (vkCreateSwapchainKHR(vulkan.device, &createInfo, nullptr, &vulkan.swapChain)
		!= VK_SUCCESS)
		throw std::runtime_error("Failed to create swap chain!");

	vkGetSwapchainImagesKHR(vulkan.device, vulkan.swapChain, &imageCount, nullptr);
	vulkan.swapChainImages.resize(imageCount);
	vkGetSwapchainImagesKHR(
		vulkan.device, vulkan.swapChain, &imageCount, vulkan.swapChainImages.data()
	);
	vulkan.swapChainImageFormat = surfaceFormat.format;
	vulkan.swapChainExtent		= extent;

	EnqueueCleanupFunc(vulkan.swapchainCleanupQueue, { (void*)&vulkan, 0, CleanupVkSwapchain });
}

// TODO: stop this from halting the entire app on minimize
void VLK::RebuildSwapchain(Vulkan& vulkan) {
	PLAT::AwaitWindowMinimize(vulkan.windowDef->window);
	vkDeviceWaitIdle(vulkan.device);
	FlushCleanupQueue(vulkan.swapchainCleanupQueue);
	BuildSwapchain(vulkan);
	InitImageViews(vulkan);
	InitColorResources(vulkan);
	InitDepthResources(vulkan);
	InitFramebuffers(vulkan);
}

void VLK::InitRenderPass(Vulkan& vulkan) {
	VkFormat swapFormat{
		SelectSurfaceFormat(QuerySwapchainSupport(vulkan.physicalDevice, vulkan.surface).formats
		)
			.format
	};

	VkAttachmentDescription colorAttachment{};
	colorAttachment.format		   = swapFormat;
	colorAttachment.samples		   = vulkan.msaaSamples;
	colorAttachment.loadOp		   = VK_ATTACHMENT_LOAD_OP_CLEAR;
	colorAttachment.storeOp		   = VK_ATTACHMENT_STORE_OP_STORE;
	colorAttachment.stencilLoadOp  = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	colorAttachment.initialLayout  = VK_IMAGE_LAYOUT_UNDEFINED;
	colorAttachment.finalLayout	   = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

	VkAttachmentReference colorAttachmentRef{};
	colorAttachmentRef.attachment = 0;
	colorAttachmentRef.layout	  = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

	// can't present multisampled image directly, need to convert to regular image
	VkAttachmentDescription colorAttachmentResolve{};
	colorAttachmentResolve.format		  = swapFormat;
	colorAttachmentResolve.samples		  = VK_SAMPLE_COUNT_1_BIT;
	colorAttachmentResolve.loadOp		  = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	colorAttachmentResolve.storeOp		  = VK_ATTACHMENT_STORE_OP_STORE;
	colorAttachmentResolve.stencilLoadOp  = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	colorAttachmentResolve.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	colorAttachmentResolve.initialLayout  = VK_IMAGE_LAYOUT_UNDEFINED;
	colorAttachmentResolve.finalLayout	  = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

	VkAttachmentReference colorAttachmentResolveRef{};
	colorAttachmentResolveRef.attachment = 2;
	colorAttachmentResolveRef.layout	 = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

	VkAttachmentDescription depthAttachment{};
	depthAttachment.format		   = VLK::FindDepthFormat(vulkan.physicalDevice);
	depthAttachment.samples		   = vulkan.msaaSamples;
	depthAttachment.loadOp		   = VK_ATTACHMENT_LOAD_OP_CLEAR;
	depthAttachment.storeOp		   = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	depthAttachment.stencilLoadOp  = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	depthAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	depthAttachment.initialLayout  = VK_IMAGE_LAYOUT_UNDEFINED;
	depthAttachment.finalLayout	   = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

	VkAttachmentReference depthAttachmentRef{};
	depthAttachmentRef.attachment = 1;
	depthAttachmentRef.layout	  = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

	VkSubpassDescription subpass{};
	subpass.pipelineBindPoint		= VK_PIPELINE_BIND_POINT_GRAPHICS;
	subpass.colorAttachmentCount	= 1; // (location=N) out blah blah in shader
	subpass.pColorAttachments		= &colorAttachmentRef;
	subpass.pDepthStencilAttachment = &depthAttachmentRef;
	subpass.pResolveAttachments		= &colorAttachmentResolveRef;

	VkSubpassDependency dependency{};
	dependency.srcSubpass	 = VK_SUBPASS_EXTERNAL;
	dependency.dstSubpass	 = 0;
	dependency.srcAccessMask = 0;
	dependency.srcStageMask	 = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT
							| VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
	dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT
							| VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
	dependency.dstAccessMask
		= VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;

	std::array<VkAttachmentDescription, 3> attachments{ colorAttachment, depthAttachment,
														colorAttachmentResolve };
	VkRenderPassCreateInfo createInfo{};
	createInfo.sType		   = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
	createInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
	createInfo.pAttachments	   = attachments.data();
	createInfo.subpassCount	   = 1;
	createInfo.pSubpasses	   = &subpass;
	createInfo.dependencyCount = 1;
	createInfo.pDependencies   = &dependency;

	if (vkCreateRenderPass(vulkan.device, &createInfo, nullptr, &vulkan.renderPass)
		!= VK_SUCCESS)
		throw std::runtime_error("Failed to create render pass!");

	EnqueueCleanupFunc(vulkan.coreCleanupQueue, { (void*)&vulkan, 0, CleanupVkRenderPass });
}

// HELPERS

VLK::SwapchainSupportDetails
VLK::QuerySwapchainSupport(VkPhysicalDevice device, VkSurfaceKHR surface) {
	VLK::SwapchainSupportDetails details;

	vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, &details.capabilities);

	uint32_t formatCount;
	vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, nullptr);
	if (formatCount != 0) {
		details.formats.resize(formatCount);
		vkGetPhysicalDeviceSurfaceFormatsKHR(
			device, surface, &formatCount, details.formats.data()
		);
	}

	uint32_t presentModeCount;
	vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, nullptr);
	if (presentModeCount != 0) {
		details.presentModes.resize(presentModeCount);
		vkGetPhysicalDeviceSurfacePresentModesKHR(
			device, surface, &presentModeCount, details.presentModes.data()
		);
	}

	return details;
}

VkSurfaceFormatKHR
VLK::SelectSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats) {
	for (const auto& availableFormat : availableFormats)
		if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB
			&& availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
			return availableFormat;
	return availableFormats[0];
}

// NOTE: prefer VK_PRESENT_MODE_FIFO_KHR for mobile (energy use concerns)
// NOTE: does anything support VK_PRESENT_MODE_MAILBOX_KHR ???
VkPresentModeKHR
VLK::SelectPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes) {
	// bool hasImmediateMode{ false };
	// for (const auto& availablePresentMode : availablePresentModes) {
	// 	if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR)
	// 		return availablePresentMode;
	// 	if (availablePresentMode == VK_PRESENT_MODE_IMMEDIATE_KHR)
	// 		hasImmediateMode = true;
	// }
	// if (hasImmediateMode)
	// 	return VK_PRESENT_MODE_IMMEDIATE_KHR;
	return VK_PRESENT_MODE_FIFO_KHR;
}

VkExtent2D VLK::SelectExtent(Vulkan& vulkan, const VkSurfaceCapabilitiesKHR& capabilities) {
	if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()) {
		return capabilities.currentExtent;
	} else {
		int width, height;
		PLAT::GetWindowSize(vulkan.windowDef->window, &width, &height);

		VkExtent2D actualExtent{ static_cast<uint32_t>(width), static_cast<uint32_t>(height) };

		actualExtent.width = std::clamp(
			actualExtent.width, capabilities.minImageExtent.width,
			capabilities.maxImageExtent.width
		);
		actualExtent.height = std::clamp(
			actualExtent.height, capabilities.minImageExtent.height,
			capabilities.maxImageExtent.height
		);

		return actualExtent;
	}
}

// CLEANUP

void VLK::CleanupVkSwapchain(void* vulkan, uint32_t i) {
	Vulkan* v = static_cast<Vulkan*>(vulkan);
	vkDestroySwapchainKHR(v->device, v->swapChain, nullptr);
}

void VLK::CleanupVkRenderPass(void* vulkan, uint32_t i) {
	Vulkan* v = static_cast<Vulkan*>(vulkan);
	vkDestroyRenderPass(v->device, v->renderPass, nullptr);
}