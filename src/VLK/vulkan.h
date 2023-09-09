#pragma once

// FIXME: treat this area like a singleton? function args are starting to get verbose, is
// keeping the door open to run concurrent vulkan instances worth it?
// FIXME: making window too large causes crazy perf drop (2000->60fps suddely just by passing
// some threshold), why???
// FIXME: need to get rid of window def?; does the window ref need to exist after surface
// acquired?

// TODO: decide if it's worth splitting up main struct into a few broad subsystems that could
// potentially be swapped in/out with others on demand (i.e. device, swapchain, etc. structs)
// TODO: ... also more generally refactoring into fewer translation units even if not choosing
// to do the above?
// TODO: some way of configuring system to use z-up or y-up? is this even needed at the backend
// level?
// TODO: put surface stuff here instead of in platform layer
// TODO: retry implementing resource/device queues now that cleanup sludge solved
// TODO: see if frame in flight-related scheduling gates can/should be combined
// TODO:
//   - external msaa setting (limit max msaa samples)
//   - sample shading (texture antialiasing) toggle
// TODO: cleanup compiler stuff so that warning exceptions dont need to be unnecessarily passed
// at the command line level

// NOTE: do resources actually need to use unordered maps? yes because arbitrary load/unload?
// they probably don't need to use strings tho

#include <VLK/cleanup.h>
#include <VLK/types.h>
#include <platform/window.h>

#include <_incl_global.hpp>

namespace VLK {

struct Vulkan {
	uint32_t maxFramesInFlight{ 3 };

	PLAT::WindowDef* windowDef;
	glm::mat4 cameraMatrix{}; // the view-projection part of model-view-projection
	VmaAllocator allocator;

	VkInstance instance;
	VkDebugUtilsMessengerEXT debugMessenger;
	VkSurfaceKHR surface;
	VkPhysicalDevice physicalDevice{ VK_NULL_HANDLE };
	VkDevice device;
	std::vector<QueuedCleanupFunction> coreCleanupQueue;

	VkQueue graphicsQueue;
	VkQueue presentQueue;

	VkSwapchainKHR swapChain;
	VkExtent2D swapChainExtent;
	VkFormat swapChainImageFormat;
	std::vector<VkImage> swapChainImages;
	std::vector<VkImageView> swapChainImageViews;
	std::vector<VkFramebuffer> swapChainFrameBuffers;
	std::vector<QueuedCleanupFunction> swapchainCleanupQueue;

	UploadContext uploadContext;
	VkRenderPass renderPass;
	VkCommandPool commandPool;
	std::vector<VkCommandBuffer> commandBuffer;
	std::vector<VkSemaphore> imageAvailable;
	std::vector<VkSemaphore> renderFinished;
	std::vector<VkFence> inFlight;
	uint32_t currentFrame{ 0 };
	VLK::DescAllocator descAlloc{};
	VLK::DescLayoutCache descCache{};
	std::vector<VLK::DescAllocator> descAllocFrames;
	VkDescriptorPool descImgui{};

	// std::deque<std::function<void()>> resourceCleanupQueue;
	VkSampleCountFlagBits msaaSamples{ VK_SAMPLE_COUNT_1_BIT };
	AllocatedImage depthImage;
	VkImageView depthImageView;
	AllocatedImage colorImage;
	VkImageView colorImageView;
	std::unordered_map<std::string, TextureResource> textures;
	std::unordered_map<std::string, ModelResource> models;
	std::vector<RenderObject> renderObjects;
};

} // namespace VLK