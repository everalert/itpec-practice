#pragma once

#include <_incl_global.hpp>

namespace PLAT {

struct WindowDef {
	GLFWwindow* window;
	uint32_t width{ 1280 };
	uint32_t height{ 960 };
	std::string title{ "ITPEC Practice Tool" };
	bool isFramebufferResized{ false };
};

void InitWindow(WindowDef&);

void EndWindow(GLFWwindow*);

void PollWindowEvents();

bool IsWindowClosing(GLFWwindow*);

void GetWindowSize(GLFWwindow*, int32_t*, int32_t*);

void AwaitWindowMinimize(GLFWwindow*);

std::vector<const char*> GetRequiredVulkanWindowExtensions(bool);

void CreateVulkanWindowSurface(PLAT::WindowDef&, VkInstance, VkAllocationCallbacks*, VkSurfaceKHR*);

void WindowFramebufferResizeCallback(GLFWwindow*, int, int);

} // namespace PLAT