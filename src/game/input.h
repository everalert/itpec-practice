#pragma once

#include <_incl_global.hpp>

namespace GAME {

struct KeyInput {
	int32_t keycode{ GLFW_KEY_UNKNOWN };
	int32_t state{ GLFW_RELEASE };
	int32_t stateLen{ 0 };
};

struct MouseInput {
	glm::vec2 pos{};
	glm::vec2 old{};
	glm::vec2 delta{};
	glm::vec2 delta360{};
	float cm360{ 24.0f };
	float dpi{ 1600.0f };
	float sens{};
};

struct InputManager {
	GLFWwindow* window{ nullptr };
	MouseInput mouse{};
	std::unordered_map<std::string, KeyInput> keys{};
};

void InputInit(InputManager& input, GLFWwindow* window);
void InputUpdate(InputManager& input);
void InputUpdateKey(KeyInput& key, GLFWwindow* window);
void InputUpdateMouse(MouseInput& mouse, GLFWwindow* window);
void InputAddKey(InputManager& input, const std::string& key, int32_t keycode);
int32_t InputGetKey(InputManager& input, const std::string& key);

} // namespace GAME