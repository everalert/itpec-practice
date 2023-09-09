#pragma once

#include <_incl_global.hpp>
#include <game/question.h>

namespace GAME {

struct PlayerObject {
	std::vector<std::unique_ptr<Q>> questions;
	std::mt19937 rng{ std::random_device{}() };
};

void PlayerGenerateQuestions(PlayerObject& player);

} // namespace GAME