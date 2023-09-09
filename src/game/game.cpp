#include "game.h"

#include <game/input.h>
#include <game/player.h>
#include <game/ui.h>
#include <platform/imgui.h>
#include <platform/renderer.h>
#include <platform/timing.h>
#include <platform/window.h>

GAME::Game GAME::GameInit() {
	Game game{};

	PLAT::InitWindow(game.windowDef);
	game.fpsCtrl = PLAT::TimeCtrlInit(144);

	game.renderer.fonts["MainText"]		= { nullptr, "fonts/Roboto-Regular.ttf", 17 };
	game.renderer.fonts["QuestionText"] = { nullptr, "fonts/Roboto-Medium.ttf", 17 };
	game.renderer.fonts["SectionTitle"] = { nullptr, "fonts/Roboto-Medium.ttf", 19 };
	game.renderer.fonts["FormulaText"]	= { nullptr, "fonts/JetBrainsMonoNL-Regular.ttf", 18 };
	// game.renderer.fonts["JetBrainsMonoMedium18"]
	// 	= { nullptr, "fonts/JetBrainsMonoNL-Medium.ttf", 18 };
	PLAT::InitRenderer(game.renderer, game.windowDef);

	GAME::InputInit(game.input, game.windowDef.window);
	GAME::InputAddKey(game.input, "SHOW_UI", GLFW_KEY_GRAVE_ACCENT);

	GAME::UIInit(game.ui, game.windowDef.window, &game.renderer.fonts);

	GAME::PlayerGenerateQuestions(game.player);

	return game;
}

void GAME::GameEnd(Game& game) {
	PLAT::CleanupRenderer(game.renderer);
	PLAT::EndWindow(game.windowDef.window);
}

void GAME::GameUpdate(Game& game, double dt) {
	PLAT::GetWindowSize(game.windowDef.window, &game.windowW, &game.windowH);
	GAME::InputUpdate(game.input);
	PLAT::TimeCtrlUpdate(game.fpsCtrl, dt);
}

void GAME::GameRender(Game& game, float dt_f) {
	GAME::UIRender(game.ui, game.player, game.input, game.fpsCtrl, dt_f);
	PLAT::RenderFrame(game.renderer);
}
