#include "question09_corpLegal.h"

#include <game/utility.h>
#include <platform/imgui.h>

// ---
// [SECTION] Corporate Activities
// [ITEM] Depreciation - Straight-Line
// [ITEM] Future Strategy - Expected Value
// [SECTION] Legal Affairs
// ---

// [SECTION] Corporate Activities

// [ITEM] Depreciation - Straight-Line

void GAME::Question_CorpLegal_StraightLine::GenerateProblem(std::mt19937& rng) {
	m_Answer   = (rng() % 100 + 1) * 1000;
	m_Years	   = rng() % 10 + 3;
	m_Salvage  = (rng() % 100 + 1) * 1000;
	m_Purchase = m_Answer * m_Years + m_Salvage;
}

void GAME::Question_CorpLegal_StraightLine::RenderQuestion(
	std::unordered_map<std::string, PLAT::ImguiFont>& fonts
) {
	static ImGuiTableFlags tableFlags
		= ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_NoHostExtendX;
	// static ImVec2 tablePadding{ 6, 2 };
	// ImGui::PushStyleVar(ImGuiStyleVar_CellPadding, tablePadding);
	if (ImGui::BeginTable("details", 2, tableFlags)) {
		ImGui::TableNextColumn();
		ImGui::Text("Purchase cost:");
		ImGui::TableNextColumn();
		ImGui::Text("$%s", CommaFNum(m_Purchase).c_str());
		ImGui::TableNextColumn();
		ImGui::Text("Salvage value:");
		ImGui::TableNextColumn();
		ImGui::Text("$%s", CommaFNum(m_Salvage).c_str());
		ImGui::TableNextColumn();
		ImGui::Text("Time period:");
		ImGui::TableNextColumn();
		ImGui::Text("%u years", m_Years);
		ImGui::EndTable();
	}
	// ImGui::PopStyleVar();
}

void GAME::Question_CorpLegal_StraightLine::RenderAnswer(
	std::unordered_map<std::string, PLAT::ImguiFont>& fonts
) {
	ImGui::Text("$%s", CommaFNum(m_Answer).c_str());
}

void GAME::Question_CorpLegal_StraightLine::RenderHint(
	std::unordered_map<std::string, PLAT::ImguiFont>& fonts
) {
	ImGui::Text("Depreciation is linear.");
	ImGui::PushFont(fonts["FormulaText"].font);
	ImGui::Text("(purchase_cost - salvage_value) / years");
	ImGui::PopFont();
}

// [ITEM] Future Strategy - Expected Value

void GAME::Question_CorpLegal_FutureExpectedValue::GenerateProblem(std::mt19937& rng) {
	m_SaleDays		 = rng() % 4 + 3;
	m_PurchaseVolume = rng() % 7 + 6;
	m_UnitValue		 = (rng() % 19 + 2) * 10; // $20-$200
	m_UnitLoss		 = (rng() % 8 + 3);		  // $3-$10
	m_Answer		 = 0;
	int32_t rProb{ 10 }, rSales{ m_PurchaseVolume };
	for (int i = m_SaleDays - 1; i >= 0; --i) {
		m_Probabilities[i] = i == 0 ? rProb : std::min((int32_t)(rng() % 4 + 1), rProb);
		m_Sales[i]		   = rSales;
		m_Profits[i]	   = m_Sales[i] * m_UnitValue;
		m_Losses[i]		   = (m_PurchaseVolume - m_Sales[i]) * m_UnitLoss;
		m_Answer += (float)(m_Profits[i] - m_Losses[i]) / 10 * m_Probabilities[i];
		rProb  = std::max(0, rProb - m_Probabilities[i]);
		rSales = std::max(0, rSales - (int)(rng() % 3 + 1));
	}
}

void GAME::Question_CorpLegal_FutureExpectedValue::RenderQuestion(
	std::unordered_map<std::string, PLAT::ImguiFont>& fonts
) {
	static ImGuiTableFlags tableFlags1
		= ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_NoHostExtendX;
	static ImGuiTableFlags tableFlags2 = ImGuiTableFlags_SizingFixedFit
									   | ImGuiTableFlags_NoHostExtendX
									   | ImGuiTableFlags_Borders;
	// static ImVec2 tablePadding{ 6, 2 };
	// ImGui::PushStyleVar(ImGuiStyleVar_CellPadding, tablePadding);
	if (ImGui::BeginTable("FutureExpectedValueTable1", 2, tableFlags1)) {
		ImGui::TableNextColumn();
		ImGui::Text("Units purchased:");
		ImGui::TableNextColumn();
		ImGui::Text("%d", m_PurchaseVolume);
		ImGui::TableNextColumn();
		ImGui::Text("Profit per sale:");
		ImGui::TableNextColumn();
		ImGui::Text("$%d", m_UnitValue);
		ImGui::TableNextColumn();
		ImGui::Text("Loss per unsold unit:");
		ImGui::TableNextColumn();
		ImGui::Text("-$%d", m_UnitLoss);
		ImGui::EndTable();
	}
	if (ImGui::BeginTable("FutureExpectedValueTable2", m_SaleDays + 1, tableFlags2)) {
		ImGui::PushFont(fonts["QuestionText"].font);
		ImGui::TableNextColumn();
		ImGui::Text("Sales");
		for (int32_t i = 0; i < m_SaleDays; ++i) {
			ImGui::TableNextColumn();
			ImGui::Text("%d", m_Sales[i]);
		}
		ImGui::PopFont();
		ImGui::TableNextColumn();
		ImGui::Text("Probability");
		for (int32_t i = 0; i < m_SaleDays; ++i) {
			ImGui::TableNextColumn();
			ImGui::Text("%d%%", m_Probabilities[i] * 10);
		}
		ImGui::EndTable();
	}
	// ImGui::PopStyleVar();
}

void GAME::Question_CorpLegal_FutureExpectedValue::RenderHint(
	std::unordered_map<std::string, PLAT::ImguiFont>& fonts
) {
	ImGui::Text("Sum of weighted ROIs.");
	ImGui::PushFont(fonts["FormulaText"].font);
	ImGui::Text("rtn1 * prob1 + rtn2 * prob2 + rtn3 * prob3 + ...");
	ImGui::PopFont();
}

void GAME::Question_CorpLegal_FutureExpectedValue::RenderAnswer(
	std::unordered_map<std::string, PLAT::ImguiFont>& fonts
) {
	ImGui::Text("$%.2f", m_Answer);
}

// [SECTION] Legal Affairs

// ...