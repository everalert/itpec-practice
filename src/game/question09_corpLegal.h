#pragma once

#include "question.h"

namespace GAME {

// [SECTION] Corporate Activities
// [ITEM] Depreciation - Straight-Line
// [ITEM] Future Strategy - Expected Value
// [SECTION] Legal Affairs

// [SECTION] Corporate Activities

// [ITEM] Depreciation - Straight-Line

class Question_CorpLegal_StraightLine : public Q {
public:
	Question_CorpLegal_StraightLine() {
		Q::strVerb	 = "CALC";
		Q::strResult = "Annual depreciation";
		Q::strMethod = "Straight-line method";
		Q::strBookPg = "IT Strategy & Management, pp.25";
	};
	void GenerateProblem(std::mt19937& rng);
	void RenderQuestion(std::unordered_map<std::string, PLAT::ImguiFont>& fonts);
	void RenderHint(std::unordered_map<std::string, PLAT::ImguiFont>& fonts);
	void RenderAnswer(std::unordered_map<std::string, PLAT::ImguiFont>& fonts);

private:
	uint32_t m_Answer;
	uint32_t m_Purchase;
	uint32_t m_Salvage;
	uint32_t m_Years;
};

// [ITEM] Future Strategy - Expected Value

class Question_CorpLegal_FutureExpectedValue : public Q {
public:
	Question_CorpLegal_FutureExpectedValue() {
		Q::strVerb	 = "CALC";
		Q::strResult = "Expected profit value";
		Q::strMethod = "Expectation principle";
		Q::strBookPg = "IT Strategy & Management, pp.69";
	};
	void GenerateProblem(std::mt19937& rng);
	void RenderQuestion(std::unordered_map<std::string, PLAT::ImguiFont>& fonts);
	void RenderHint(std::unordered_map<std::string, PLAT::ImguiFont>& fonts);
	void RenderAnswer(std::unordered_map<std::string, PLAT::ImguiFont>& fonts);

private:
	float m_Answer;
	int32_t m_SaleDays;
	int32_t m_PurchaseVolume;
	int32_t m_UnitValue;
	int32_t m_UnitLoss;
	int32_t m_Probabilities[8];
	int32_t m_Sales[8];
	int32_t m_Profits[8];
	int32_t m_Losses[8];
};

// [SECTION] Legal Affairs

// ...

} // namespace GAME