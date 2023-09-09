#pragma once

#include "utility.h"

#include <platform/imgui.h>

#include <_incl_global.hpp>

namespace GAME {

class Q {
public:
	virtual ~Q(){};
	virtual void GenerateProblem(std::mt19937& rng)										 = 0;
	virtual void RenderQuestion(std::unordered_map<std::string, PLAT::ImguiFont>& fonts) = 0;
	virtual void RenderHint(std::unordered_map<std::string, PLAT::ImguiFont>& fonts)	 = 0;
	virtual void RenderAnswer(std::unordered_map<std::string, PLAT::ImguiFont>& fonts)	 = 0;

	// instruction text
	const char* strVerb	  = "";
	const char* strResult = "";
	const char* strMethod = "";
	const char* strBookPg = "";
	
	// state
	bool marked{ false };
	bool markedCorrect{ false };
	bool showAnswer{ false };
	bool showHint{ false };
	void SetMarked(bool isMarked, bool isCorrect) {
		marked		  = isMarked;
		markedCorrect = isCorrect;
	};
	void SetShowAnswer(bool show) { showAnswer = show; };
	void SetShowHint(bool show) { showHint = show; };
};

} // namespace GAME