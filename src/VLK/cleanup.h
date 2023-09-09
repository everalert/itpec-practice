#pragma once

#include <_incl_global.hpp>

namespace VLK {

struct QueuedCleanupFunction {
	void* vulkan;
	uint32_t i;
	void (*f)(void*, uint32_t);
	// std::function<void(void*, uint32_t)> f;
};

void EnqueueCleanupFunc(
	std::vector<QueuedCleanupFunction>& queue, QueuedCleanupFunction&& function
);
void FlushCleanupQueue(std::vector<QueuedCleanupFunction>& queue);

} // namespace VLK