#include "cleanup.h"

void VLK::EnqueueCleanupFunc(
	std::vector<QueuedCleanupFunction>& queue, QueuedCleanupFunction&& function
){
	queue.push_back(function);
}

void VLK::FlushCleanupQueue(std::vector<QueuedCleanupFunction>& queue) {
	for (auto it = queue.rbegin(); it != queue.rend(); it++)
		(*it).f((*it).vulkan, (*it).i);
	queue.clear();
}