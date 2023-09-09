#include "window.h"

void PLAT::InitWindow(PLAT::WindowDef& def) {
	glfwInit();
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
	glfwWindowHint(GLFW_MAXIMIZED, GLFW_TRUE);
	def.window = glfwCreateWindow(def.width, def.height, def.title.c_str(), nullptr, nullptr);
	glfwSetWindowUserPointer(def.window, &def); // access owner from within callbacks
	glfwSetFramebufferSizeCallback(def.window, PLAT::WindowFramebufferResizeCallback);
}

void PLAT::EndWindow(GLFWwindow* window) {
	glfwDestroyWindow(window);
	glfwTerminate();
}

void PLAT::PollWindowEvents() {
	glfwPollEvents();
}

bool PLAT::IsWindowClosing(GLFWwindow* window) {
	return glfwWindowShouldClose(window);
}

void PLAT::GetWindowSize(GLFWwindow* window, int32_t* width, int32_t* height) {
	glfwGetFramebufferSize(window, width, height);
}

void PLAT::AwaitWindowMinimize(GLFWwindow* window) {
	int width{ 0 }, height{ 0 };
	GetWindowSize(window, &width, &height);
	while (width == 0 || height == 0) {
		GetWindowSize(window, &width, &height);
		glfwWaitEvents();
	}
}

std::vector<const char*> PLAT::GetRequiredVulkanWindowExtensions(bool enableValidationLayers) {
	uint32_t glfwExtensionCount{ 0 };
	const char** glfwExtensions{ glfwGetRequiredInstanceExtensions(&glfwExtensionCount) };

	std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

	if (enableValidationLayers)
		extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);

	return extensions;
}

void PLAT::CreateVulkanWindowSurface(
	PLAT::WindowDef& windowDef, VkInstance instance, VkAllocationCallbacks* allocator,
	VkSurfaceKHR* surface
) {
	if (glfwCreateWindowSurface(instance, windowDef.window, allocator, surface) != VK_SUCCESS)
		throw std::runtime_error("Failed to create window surface!");
}

void PLAT::WindowFramebufferResizeCallback(
	GLFWwindow* window, [[maybe_unused]] int width, [[maybe_unused]] int height
) {
	auto app = reinterpret_cast<PLAT::WindowDef*>(glfwGetWindowUserPointer(window));
	app->isFramebufferResized = true;
}