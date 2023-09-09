#pragma once

#include <_incl_global.hpp>

namespace PLAT {

struct Timer {
	std::chrono::steady_clock::time_point start;
	std::chrono::steady_clock::time_point prev;
	std::chrono::steady_clock::time_point current;
	double dt{};
	float dt_f{};
};

Timer TimerInit();

void TimerUpdate(Timer& timer);

struct TimeControl {
	double accumulator{ 0.0 };
	int32_t fixedFPS{ 60 };
	double fixedStep{ 1.0 / 60.0 };
	double maxStep{ 1.0 };
	double minStep{ 0.0 };
};

TimeControl TimeCtrlInit(int32_t fps = 60, double maxStep = 1.0, double minStep = 0.0);

void TimeCtrlUpdate(TimeControl& t, double step);

bool TimeCtrlConsume(TimeControl& t);

void TimeCtrlSetFPS(TimeControl& t, int32_t fps);

} // namespace PLAT