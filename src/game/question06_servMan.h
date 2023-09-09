#pragma once

// [SECTION] Service Management
// [ITEM] System Evaluation - MTBF
// [SECTION] System Auditing

#include "question.h"

namespace GAME {

// [SECTION] Service Management

// [ITEM] System Evaluation - MTBF

class Question_ServMan_MTBF : public Q {
public:
	Question_ServMan_MTBF() {
		Q::strVerb	 = "CALC";
		Q::strResult = "System uptime";
		Q::strMethod = "MTBF (hours)";
		Q::strBookPg = "IT Fundamentals, pp.144";
	};
	void GenerateProblem(std::mt19937& rng);
	void RenderQuestion(std::unordered_map<std::string, PLAT::ImguiFont>& fonts);
	void RenderHint(std::unordered_map<std::string, PLAT::ImguiFont>& fonts);
	void RenderAnswer(std::unordered_map<std::string, PLAT::ImguiFont>& fonts);

private:
	float m_Answer;
	int32_t m_Days;
	int32_t m_DayPlannedUptime;
	int32_t m_DowntimeCount;
	float m_DowntimeHours[8];
};

// [SECTION] System Auditing

} // namespace GAME
