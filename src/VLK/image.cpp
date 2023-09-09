#include "image.h"

#include <VLK/cleanup.h>
#include <VLK/command.h>
#include <VLK/initializers.hpp>
#include <VLK/memory.h>
#include <VLK/vulkan.h>

// INITIALIZATION

void VLK::InitImageViews(VLK::Vulkan& vulkan) {
	vulkan.swapChainImageViews.resize(vulkan.swapChainImages.size());
	for (uint32_t i = 0; i < vulkan.swapChainImages.size(); ++i) {
		vulkan.swapChainImageViews[i] = CreateImageView(
			vulkan.device, vulkan.swapChainImages[i], vulkan.swapChainImageFormat,
			VK_IMAGE_ASPECT_COLOR_BIT, 1
		);
		EnqueueCleanupFunc(
			vulkan.swapchainCleanupQueue, { (void*)&vulkan, i, CleanupVkImageViewSwap }
		);
	}
}

void VLK::InitColorResources(VLK::Vulkan& vulkan) {
	CreateImage(
		vulkan.swapChainExtent.width, vulkan.swapChainExtent.height, 1, vulkan.msaaSamples,
		vulkan.swapChainImageFormat, VK_IMAGE_TILING_OPTIMAL,
		VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT | VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
		vulkan.allocator, VMA_MEMORY_USAGE_CPU_TO_GPU, vulkan.colorImage
	);
	EnqueueCleanupFunc(
		vulkan.swapchainCleanupQueue, { (void*)&vulkan, 0, CleanupVkImageColor }
	);
	vulkan.colorImageView = CreateImageView(
		vulkan.device, vulkan.colorImage.image, vulkan.swapChainImageFormat,
		VK_IMAGE_ASPECT_COLOR_BIT, 1
	);
	EnqueueCleanupFunc(
		vulkan.swapchainCleanupQueue, { (void*)&vulkan, 0, CleanupVkImageViewColor }
	);
}

void VLK::InitDepthResources(VLK::Vulkan& vulkan) {
	VkFormat depthFormat{ FindDepthFormat(vulkan.physicalDevice) };
	CreateImage(
		vulkan.swapChainExtent.width, vulkan.swapChainExtent.height, 1, vulkan.msaaSamples,
		depthFormat, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,
		vulkan.allocator, VMA_MEMORY_USAGE_CPU_TO_GPU, vulkan.depthImage
	);
	EnqueueCleanupFunc(
		vulkan.swapchainCleanupQueue, { (void*)&vulkan, 0, CleanupVkImageDepth }
	);
	vulkan.depthImageView = CreateImageView(
		vulkan.device, vulkan.depthImage.image, depthFormat, VK_IMAGE_ASPECT_DEPTH_BIT, 1
	);
	EnqueueCleanupFunc(
		vulkan.swapchainCleanupQueue, { (void*)&vulkan, 0, CleanupVkImageViewDepth }
	);

	TransitionImageLayout(
		vulkan, vulkan.depthImage.image, depthFormat, VK_IMAGE_LAYOUT_UNDEFINED,
		VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL, 1
	);
}

void VLK::InitFramebuffers(VLK::Vulkan& vulkan) {
	vulkan.swapChainFrameBuffers.resize(vulkan.swapChainImageViews.size());

	for (uint32_t i = 0; i < vulkan.swapChainImageViews.size(); ++i) {
		std::array<VkImageView, 3> attachments{
			vulkan.colorImageView,
			vulkan.depthImageView,
			vulkan.swapChainImageViews[i],
		};

		VkFramebufferCreateInfo framebufferInfo{};
		framebufferInfo.sType			= VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		framebufferInfo.renderPass		= vulkan.renderPass;
		framebufferInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
		framebufferInfo.pAttachments	= attachments.data();
		framebufferInfo.width			= vulkan.swapChainExtent.width;
		framebufferInfo.height			= vulkan.swapChainExtent.height;
		framebufferInfo.layers			= 1;

		if (vkCreateFramebuffer(
				vulkan.device, &framebufferInfo, nullptr, &vulkan.swapChainFrameBuffers[i]
			)
			!= VK_SUCCESS)
			throw std::runtime_error("Failed to create framebuffer!");

		EnqueueCleanupFunc(
			vulkan.swapchainCleanupQueue, { (void*)&vulkan, i, CleanupVkFramebuffer }
		);
	}
}

// RESOURCES

void VLK::CreateImage(
	VmaAllocator allocator, VmaMemoryUsage allocUsage, VkImageCreateInfo& imageInfo,
	VLK::AllocatedImage& image
) {
	VmaAllocationCreateInfo allocInfo{};
	allocInfo.usage = allocUsage;

	if (vmaCreateImage(
			allocator, &imageInfo, &allocInfo, &image.image, &image.allocation, nullptr
		)
		!= VK_SUCCESS)
		throw std::runtime_error("Failed to create image!");
}

void VLK::CreateImage(
	uint32_t width, uint32_t height, uint32_t mipLevels, VkSampleCountFlagBits numSamples,
	VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VmaAllocator allocator,
	VmaMemoryUsage allocUsage, AllocatedImage& image
) {
	auto imageInfo{
		VLK::MakeImageCI(width, height, mipLevels, numSamples, format, tiling, usage)
	};

	VmaAllocationCreateInfo allocInfo{};
	allocInfo.usage = allocUsage;

	if (vmaCreateImage(
			allocator, &imageInfo, &allocInfo, &image.image, &image.allocation, nullptr
		)
		!= VK_SUCCESS)
		throw std::runtime_error("Failed to create image!");
}

VkImageView VLK::CreateImageView(
	VkDevice device, VkImage image, VkFormat format, VkImageAspectFlags aspectFlags,
	uint32_t mipLevels
) {
	VkImageViewCreateInfo createInfo{};
	createInfo.sType			= VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
	createInfo.image			= image;
	createInfo.viewType			= VK_IMAGE_VIEW_TYPE_2D;
	createInfo.format			= format;
	createInfo.subresourceRange = { aspectFlags, 0, mipLevels, 0, 1 };
	// NOTE: subresource layercount >1 for stereoscopic 3D

	VkImageView imageView;
	if (vkCreateImageView(device, &createInfo, nullptr, &imageView) != VK_SUCCESS)
		throw std::runtime_error("Failed to create image view!");

	return imageView;
}

// TODO: load mipmaps from a texture that precomputed them instead of making them at runtime
void VLK::GenerateMipmaps(
	Vulkan& vulkan,

	VkImage image, VkFormat imageFormat, uint32_t texWidth, uint32_t texHeight,
	uint32_t mipLevels
) {
	VkFormatProperties formatProperties{};
	vkGetPhysicalDeviceFormatProperties(vulkan.physicalDevice, imageFormat, &formatProperties);
	if (!(formatProperties.optimalTilingFeatures
		  & VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT))
		throw std::runtime_error("Texture image format does not support linear blitting!");

	SendImmediateCommand(vulkan, [&](VkCommandBuffer cmd) {
		VkImageMemoryBarrier barrier{};
		barrier.sType				= VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
		barrier.image				= image;
		barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		barrier.subresourceRange	= { VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1 };

		int32_t mipWidth{ static_cast<int32_t>(texWidth) },
			mipHeight{ static_cast<int32_t>(texHeight) };
		for (uint32_t i = 1; i < mipLevels; ++i) {
			barrier.subresourceRange.baseMipLevel = i - 1;
			barrier.oldLayout					  = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
			barrier.newLayout					  = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
			barrier.srcAccessMask				  = VK_ACCESS_TRANSFER_WRITE_BIT;
			barrier.dstAccessMask				  = VK_ACCESS_TRANSFER_READ_BIT;

			vkCmdPipelineBarrier(
				cmd, VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT, 0, 0,
				nullptr, 0, nullptr, 1, &barrier
			);

			VkImageBlit blit{};
			blit.srcOffsets[0]	= { 0, 0, 0 };
			blit.srcOffsets[1]	= { mipWidth, mipHeight, 1 };
			blit.srcSubresource = { VK_IMAGE_ASPECT_COLOR_BIT, i - 1, 0, 1 };
			blit.dstOffsets[0]	= { 0, 0, 0 };
			blit.dstOffsets[1]
				= { mipWidth > 1 ? mipWidth / 2 : 1, mipHeight > 1 ? mipHeight / 2 : 1, 1 };
			blit.dstSubresource = { VK_IMAGE_ASPECT_COLOR_BIT, i, 0, 1 };

			vkCmdBlitImage(
				cmd, image, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, image,
				VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &blit, VK_FILTER_LINEAR
			);

			barrier.oldLayout	  = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
			barrier.newLayout	  = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
			barrier.srcAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
			barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

			vkCmdPipelineBarrier(
				cmd, VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0,
				0, nullptr, 0, nullptr, 1, &barrier
			);

			if (mipWidth > 1)
				mipWidth /= 2;
			if (mipHeight > 1)
				mipHeight /= 2;
		}

		barrier.subresourceRange.baseMipLevel = mipLevels - 1;
		barrier.oldLayout					  = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
		barrier.newLayout					  = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		barrier.srcAccessMask				  = VK_ACCESS_TRANSFER_READ_BIT;
		barrier.dstAccessMask				  = VK_ACCESS_SHADER_READ_BIT;

		vkCmdPipelineBarrier(
			cmd, VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0, 0,
			nullptr, 0, nullptr, 1, &barrier
		);
	});
}

// HELPERS

void VLK::CopyBufferToImage(
	Vulkan& vulkan, VkBuffer buffer, VkImage image, uint32_t width, uint32_t height
) {
	SendImmediateCommand(vulkan, [&](VkCommandBuffer cmd) {
		VkBufferImageCopy region{};
		region.bufferOffset		 = 0;
		region.bufferRowLength	 = 0;
		region.bufferImageHeight = 0;
		region.imageSubresource	 = { VK_IMAGE_ASPECT_COLOR_BIT, 0, 0, 1 };
		region.imageOffset		 = { 0, 0, 0 };
		region.imageExtent		 = { width, height, 1 };

		vkCmdCopyBufferToImage(
			cmd, buffer, image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &region
		);
	});
}

void VLK::WriteImageViaStagingBuffer(
	Vulkan& vulkan, size_t size, void* data, VkImageCreateInfo& imageInfo,
	AllocatedImage& buffer
) {
	VLK::AllocatedBuffer stagingBuffer;
	CreateBuffer(
		vulkan, static_cast<uint32_t>(size), VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
		VMA_MEMORY_USAGE_CPU_ONLY, stagingBuffer
	);
	void* stagingData;
	vmaMapMemory(vulkan.allocator, stagingBuffer.allocation, &stagingData);
	memcpy(stagingData, data, size);
	vmaUnmapMemory(vulkan.allocator, stagingBuffer.allocation);

	CreateImage(vulkan.allocator, VMA_MEMORY_USAGE_GPU_ONLY, imageInfo, buffer);

	TransitionImageLayout(
		vulkan, buffer.image, imageInfo.format, VK_IMAGE_LAYOUT_UNDEFINED,
		VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, imageInfo.mipLevels
	);
	CopyBufferToImage(
		vulkan, stagingBuffer.buffer, buffer.image, imageInfo.extent.width,
		imageInfo.extent.height
	);

	vmaDestroyBuffer(vulkan.allocator, stagingBuffer.buffer, stagingBuffer.allocation);
}

void VLK::TransitionImageLayout(
	Vulkan& vulkan, VkImage image, VkFormat format, VkImageLayout oldLayout,
	VkImageLayout newLayout, uint32_t mipLevels
) {
	SendImmediateCommand(vulkan, [&](VkCommandBuffer cmd) {
		VkPipelineStageFlags srcStage, dstStage;

		VkImageMemoryBarrier barrier{};
		barrier.sType				= VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
		barrier.oldLayout			= oldLayout;
		barrier.newLayout			= newLayout;
		barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		barrier.image				= image;
		barrier.subresourceRange	= { VK_IMAGE_ASPECT_COLOR_BIT, 0, mipLevels, 0, 1 };

		if (newLayout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL) {
			barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
			if (HasStencilComponent(format))
				barrier.subresourceRange.aspectMask |= VK_IMAGE_ASPECT_STENCIL_BIT;
		} else
			barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;

		if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED
			&& newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL) {
			barrier.srcAccessMask = 0;
			barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
			srcStage			  = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
			dstStage			  = VK_PIPELINE_STAGE_TRANSFER_BIT;
		} else if (oldLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL) {
			barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
			barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
			srcStage			  = VK_PIPELINE_STAGE_TRANSFER_BIT;
			dstStage			  = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
		} else if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL) {
			barrier.srcAccessMask = 0;
			barrier.dstAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT
								  | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
			srcStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
			dstStage = VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
		} else
			throw std::invalid_argument("unsupported layout transition!");

		vkCmdPipelineBarrier(cmd, srcStage, dstStage, 0, 0, nullptr, 0, nullptr, 1, &barrier);
	});
}

VkFormat VLK::FindSupportedFormat(
	VkPhysicalDevice physicalDevice, const std::vector<VkFormat>& candidates,
	VkImageTiling tiling, VkFormatFeatureFlags features
) {
	for (VkFormat format : candidates) {
		VkFormatProperties props;
		vkGetPhysicalDeviceFormatProperties(physicalDevice, format, &props);
		if (tiling == VK_IMAGE_TILING_LINEAR
			&& (props.linearTilingFeatures & features) == features)
			return format;
		else if (tiling == VK_IMAGE_TILING_OPTIMAL && (props.optimalTilingFeatures & features) == features)
			return format;
	}
	throw std::runtime_error("Failed to find supported format!");
}

VkFormat VLK::FindDepthFormat(VkPhysicalDevice physicalDevice) {
	return FindSupportedFormat(
		physicalDevice,
		{ VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT },
		VK_IMAGE_TILING_OPTIMAL, VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT
	);
}

bool VLK::HasStencilComponent(VkFormat format) {
	return format == VK_FORMAT_D32_SFLOAT_S8_UINT || format == VK_FORMAT_D24_UNORM_S8_UINT;
}

// CLEANUP

void VLK::CleanupVkImageViewSwap(void* vulkan, uint32_t i) {
	Vulkan* v = static_cast<Vulkan*>(vulkan);
	vkDestroyImageView(v->device, v->swapChainImageViews[i], nullptr);
}
void VLK::CleanupVkImageViewColor(void* vulkan, uint32_t i) {
	Vulkan* v = static_cast<Vulkan*>(vulkan);
	vkDestroyImageView(v->device, v->colorImageView, nullptr);
}
void VLK::CleanupVkImageViewDepth(void* vulkan, uint32_t i) {
	Vulkan* v = static_cast<Vulkan*>(vulkan);
	vkDestroyImageView(v->device, v->depthImageView, nullptr);
}

void VLK::CleanupVkImageColor(void* vulkan, uint32_t i) {
	Vulkan* v = static_cast<Vulkan*>(vulkan);
	vmaDestroyImage(v->allocator, v->colorImage.image, v->colorImage.allocation);
}
void VLK::CleanupVkImageDepth(void* vulkan, uint32_t i) {
	Vulkan* v = static_cast<Vulkan*>(vulkan);
	vmaDestroyImage(v->allocator, v->depthImage.image, v->depthImage.allocation);
}

void VLK::CleanupVkFramebuffer(void* vulkan, uint32_t i) {
	Vulkan* v = static_cast<Vulkan*>(vulkan);
	vkDestroyFramebuffer(v->device, v->swapChainFrameBuffers[i], nullptr);
}