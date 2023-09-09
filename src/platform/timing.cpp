#include "timing.h"

PLAT::Timer PLAT::TimerInit() {
	Timer t{};
	t.start	  = std::chrono::high_resolution_clock::now();
	t.prev	  = t.start;
	t.current = t.prev;
	return t;
}

void PLAT::TimerUpdate(Timer& t) {
	t.current = std::chrono::high_resolution_clock::now();
	t.dt	  = std::chrono::duration<double, std::chrono::seconds::period>(t.current - t.prev)
			   .count();
	t.dt_f = (float)t.dt;
	t.prev = t.current;
}

PLAT::TimeControl PLAT::TimeCtrlInit(int32_t fps, double maxStep, double minStep) {
	TimeControl t{};
	TimeCtrlSetFPS(t, fps);
	t.minStep = minStep;
	t.maxStep = maxStep;
	return t;
}

void PLAT::TimeCtrlUpdate(TimeControl& t, double step) {
	t.accumulator = std::min(t.accumulator + step, t.maxStep);
}

bool PLAT::TimeCtrlConsume(TimeControl& t) {
	if (t.accumulator >= t.fixedStep) {
		t.accumulator -= t.fixedStep;
		return true;
	}
	return false;
}

void PLAT::TimeCtrlSetFPS(TimeControl& t, int32_t fps) {
	assert(fps > 0 && "TimeCtrlSetFPS :: FPS must be greater than 0.\n");
	t.fixedFPS = fps;
	t.fixedStep = 1.0 / (double)fps;
}