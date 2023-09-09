#pragma once

#include <game/input.h>
#include <game/player.h>
#include <game/ui.h>
#include <platform/renderer.h>
#include <platform/timing.h>
#include <platform/window.h>

namespace GAME {

struct Game {
	GAME::PlayerObject player{};
	GAME::InputManager input{};
	GAME::UIState ui{};
	PLAT::Renderer renderer{};
	PLAT::TimeControl fpsCtrl{};
	PLAT::WindowDef windowDef{};
	int windowW, windowH;
};

Game GameInit();

void GameEnd(Game& game);

void GameUpdate(Game& game, double dt);

void GameRender(Game& game, float dt_f);

} // namespace GAME