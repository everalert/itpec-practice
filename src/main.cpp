// FIXME: consider the following when reusing/porting this codebase
// NOTE: changed random glfwGetFramebufferSize's to platform/window@GetWindowSize
// NOTE: modified VLK/swapchain.cpp@SelectPresentMode to always return FIFO
// NOTE: modified VLK/command.cpp@SendRenderCommand clear color, disabled 3d stuff

#include <game/game.h>
#include <platform/timing.h>
#include <platform/window.h>

#include <_incl_global.hpp>

int main() {

	try {
		auto game{ GAME::GameInit() };
		GLFWwindow*& window = game.windowDef.window;

		auto timer{ PLAT::TimerInit() };

		while (!PLAT::IsWindowClosing(window)) {
			PLAT::TimerUpdate(timer);
			PLAT::PollWindowEvents();
			GAME::GameUpdate(game, timer.dt);
			GAME::GameRender(game, timer.dt_f);
		}

		GAME::GameEnd(game);
	} catch (const std::exception& e) {
		std::cerr << e.what() << std::endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}