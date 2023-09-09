#pragma once

#include <game/input.h>
#include <game/player.h>
#include <platform/imgui.h>
#include <platform/timing.h>

#include <_incl_global.hpp>

namespace GAME {

static bool ShowUI;
struct UIState {
	GLFWwindow* window;
	std::unordered_map<std::string, PLAT::ImguiFont>* fonts;
};

void UIInit(
	UIState& ui, GLFWwindow* window, std::unordered_map<std::string, PLAT::ImguiFont>* fonts
);
void UIRender(
	UIState& ui, PlayerObject& player, InputManager& input, PLAT::TimeControl& fpsCtrl, float dt
);

void UIDebug(UIState& ui, int winW, int winH, float dt);

} // namespace GAME