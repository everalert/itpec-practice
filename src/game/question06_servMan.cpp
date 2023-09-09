#include "question06_servMan.h"

// [SECTION] Service Management
// [ITEM] System Evaluation - MTBF
// [SECTION] System Auditing

#include <game/utility.h>
#include <platform/imgui.h>

// [SECTION] Service Management

// [ITEM] System Evaluation - MTBF

void GAME::Question_ServMan_MTBF::GenerateProblem(std::mt19937& rng) {
	m_Days			   = (rng() % 20 + 1) * 10;
	m_DayPlannedUptime = rng() % 7 + 18;
	m_DowntimeCount	   = rng() % 8 + 1;
	float totalDowntime{ 0.0f };
	for (int i = 0; i < m_DowntimeCount; ++i) {
		m_DowntimeHours[i] = (float)(rng() % 40 + 1) / 10;
		totalDowntime += m_DowntimeHours[i];
	}
	m_Answer = ((float)(m_Days * m_DayPlannedUptime) - totalDowntime) / m_DowntimeCount;
}

void GAME::Question_ServMan_MTBF::RenderQuestion(
	std::unordered_map<std::string, PLAT::ImguiFont>& fonts
) {
	static ImGuiTableFlags tableFlags
		= ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_NoHostExtendX;
	if (ImGui::BeginTable("ServMan_MTBF_question", 2, tableFlags)) {
		ImGui::TableNextColumn();
		ImGui::Text("Planned daily uptime:");
		ImGui::TableNextColumn();
		ImGui::Text("%d hours", m_DayPlannedUptime);
		ImGui::TableNextColumn();
		ImGui::Text("Operational period:");
		ImGui::TableNextColumn();
		ImGui::Text("%d days", m_Days);
		ImGui::TableNextColumn();
		ImGui::Text("Downtime instances:");
		ImGui::TableNextColumn();
		for (int i = 0; i < m_DowntimeCount; ++i) {
			if (i > 0)
				ImGui::SameLine();
			ImGui::Text("%.1fh  ", m_DowntimeHours[i]);
		}
		ImGui::EndTable();
	}
}

void GAME::Question_ServMan_MTBF::RenderHint(
	std::unordered_map<std::string, PLAT::ImguiFont>& fonts
) {
	ImGui::Text("Average realized uptime per repair cycle.");
	ImGui::PushFont(fonts["FormulaText"].font);
	ImGui::Text("(total_uptime_planned - total_downtime) / downtime_count");
	ImGui::PopFont();
}

void GAME::Question_ServMan_MTBF::RenderAnswer(
	std::unordered_map<std::string, PLAT::ImguiFont>& fonts
) {
	ImGui::Text("%.2f hours", m_Answer);
}

// [SECTION] System Auditing