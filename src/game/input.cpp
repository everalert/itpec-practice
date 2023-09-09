#include "input.h"

void GAME::InputInit(InputManager& input, GLFWwindow* window) {
	input.window = window;
	if (glfwRawMouseMotionSupported())
		glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
	glfwSetInputMode(
		window, GLFW_STICKY_KEYS, GLFW_TRUE
	); // only change reported state when key does
	InputUpdate(input);
}

void GAME::InputUpdate(InputManager& input) {
	InputUpdateMouse(input.mouse, input.window);
	for (auto& key : input.keys)
		InputUpdateKey(key.second, input.window);
}

void GAME::InputUpdateKey(KeyInput& key, GLFWwindow* window) {
	static int32_t newState{ GLFW_KEY_UNKNOWN };
	newState = glfwGetKey(window, key.keycode);
	if (key.state != newState) {
		key.state	 = newState;
		key.stateLen = 0;
	} else {
		++key.stateLen;
	}
}

void GAME::InputUpdateMouse(MouseInput& mouse, GLFWwindow* window) {
	mouse.old = mouse.pos;
	static double newX, newY;
	glfwGetCursorPos(window, &newX, &newY);
	mouse.pos	   = { (float)newX, (float)newY };
	mouse.delta	   = mouse.pos - mouse.old;
	mouse.sens	   = mouse.cm360 / 2.54f * mouse.dpi; // mouse units per 360deg rotation
	mouse.delta360 = mouse.delta / mouse.sens * 360.0f;
}

void GAME::InputAddKey(InputManager& input, const std::string& key, int32_t keycode) {
	input.keys[key]			= {};
	input.keys[key].keycode = keycode;
	InputUpdateKey(input.keys[key], input.window);
}

int32_t GAME::InputGetKey(InputManager& input, const std::string& key) {
	bool state{ input.keys[key].state > 0 };
	bool held{ input.keys[key].stateLen > 0 };
	return state ? (held ? 2 : 1) : (held ? -1 : 0);
}
