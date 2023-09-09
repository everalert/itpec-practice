#include "question02_compSys.h"

#include "utility.h"

// [SECTION] Computer Components

// ...

// [SECTION] System Components

// ...

// [SECTION] Software

// [SECTION] Hardware

// [ITEM] Individual System Availability

void GAME::Question_CompSys_SysAvailIndividual::GenerateProblem(std::mt19937& rng) {
	m_UptimeDays		= rng() % 10 + 21;
	m_UptimeHoursPerDay = rng() % 7 + 18;
	m_MTTR				= rng() % 4 + 3;
	m_MTBF				= m_UptimeDays * m_UptimeHoursPerDay;
	m_Answer			= (float)m_MTBF / (m_MTTR + m_MTBF);
}

void GAME::Question_CompSys_SysAvailIndividual::RenderQuestion(
	std::unordered_map<std::string, PLAT::ImguiFont>& fonts
) {
	static ImGuiTableFlags tableFlags
		= ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_NoHostExtendX;
	if (ImGui::BeginTable("CompSys_SysAvailIndividual_question", 2, tableFlags)) {
		ImGui::TableNextColumn();
		ImGui::Text("Uptime period");
		ImGui::TableNextColumn();
		ImGui::Text("%u days", m_UptimeDays);
		ImGui::TableNextColumn();
		ImGui::Text("Daily uptime");
		ImGui::TableNextColumn();
		ImGui::Text("%u hours", m_UptimeHoursPerDay);
		ImGui::TableNextColumn();
		ImGui::Text("Repair time");
		ImGui::TableNextColumn();
		ImGui::Text("%u hours", m_MTTR);
		ImGui::EndTable();
	}
}

void GAME::Question_CompSys_SysAvailIndividual::RenderHint(
	std::unordered_map<std::string, PLAT::ImguiFont>& fonts
) {
	ImGui::Text("The ratio of the uptime to the total cycle time.");
	ImGui::PushFont(fonts["FormulaText"].font);
	ImGui::Text("total_availability = MTBF / (MTBF + MTTR)");
	ImGui::PopFont();
}

void GAME::Question_CompSys_SysAvailIndividual::RenderAnswer(
	std::unordered_map<std::string, PLAT::ImguiFont>& fonts
) {
	ImGui::Text("%.2f%%", m_Answer * 100);
}

// [ITEM] Serial System Availability

void GAME::Question_CompSys_SysAvailSerial::GenerateProblem(std::mt19937& rng) {
	m_Answer	  = 1.0f;
	m_SystemCount = rng() % 3 + 4;
	for (uint8_t i = 0; i < m_SystemCount; ++i) {
		m_Systems[i] = (float)(rng() % 5 + 5) / 10;
		m_Answer *= m_Systems[i];
	}
}

void GAME::Question_CompSys_SysAvailSerial::RenderQuestion(
	std::unordered_map<std::string, PLAT::ImguiFont>& fonts
) {
	static ImGuiTableFlags tableFlags = ImGuiTableFlags_SizingFixedFit
									  | ImGuiTableFlags_NoHostExtendX | ImGuiTableFlags_Borders;
	if (ImGui::BeginTable("CompSys_SysAvailSerial_question", m_SystemCount, tableFlags)) {
		ImGui::PushFont(fonts["FormulaText"].font);
		for (uint8_t i = 0; i < m_SystemCount; ++i) {
			ImGui::TableNextColumn();
			ImGui::Text("%.0f%%", m_Systems[i] * 100);
		}
		ImGui::PopFont();
		ImGui::EndTable();
	}
}

void GAME::Question_CompSys_SysAvailSerial::RenderHint(
	std::unordered_map<std::string, PLAT::ImguiFont>& fonts
) {
	ImGui::Text("Multiply the availability of all components.");
	ImGui::PushFont(fonts["FormulaText"].font);
	ImGui::Text("total_availability = avail_1 * avail_2 * ...");
	ImGui::PopFont();
}

void GAME::Question_CompSys_SysAvailSerial::RenderAnswer(
	std::unordered_map<std::string, PLAT::ImguiFont>& fonts
) {
	ImGui::Text("%.2f%%", m_Answer * 100);
}

// [ITEM] Parallel System Availability

void GAME::Question_CompSys_SysAvailParallel::GenerateProblem(std::mt19937& rng) {
	m_Answer	  = 1.0f;
	m_SystemCount = rng() % 3 + 4;
	for (uint8_t i = 0; i < m_SystemCount; ++i) {
		m_Systems[i] = (float)(rng() % 5 + 5) / 10;
		m_Answer *= (1.0f - m_Systems[i]);
	}
	m_Answer = 1.0f - m_Answer;
}

void GAME::Question_CompSys_SysAvailParallel::RenderQuestion(
	std::unordered_map<std::string, PLAT::ImguiFont>& fonts
) {
	static ImGuiTableFlags tableFlags = ImGuiTableFlags_SizingFixedFit
									  | ImGuiTableFlags_NoHostExtendX | ImGuiTableFlags_Borders;
	if (ImGui::BeginTable("CompSys_SysAvailParallel_question", 1, tableFlags)) {
		ImGui::PushFont(fonts["FormulaText"].font);
		for (uint8_t i = 0; i < m_SystemCount; ++i) {
			ImGui::TableNextColumn();
			ImGui::Text("%.0f%%", m_Systems[i] * 100);
		}
		ImGui::PopFont();
		ImGui::EndTable();
	}
}

void GAME::Question_CompSys_SysAvailParallel::RenderHint(
	std::unordered_map<std::string, PLAT::ImguiFont>& fonts
) {
	ImGui::Text("Multiply the unavailability of all components and return the inverse.");
	ImGui::PushFont(fonts["FormulaText"].font);
	ImGui::Text("total_availability = 1 - ( (1 - avail_1) * (1 - avail_2) * ...)");
	ImGui::PopFont();
}

void GAME::Question_CompSys_SysAvailParallel::RenderAnswer(
	std::unordered_map<std::string, PLAT::ImguiFont>& fonts
) {
	ImGui::Text("%.2f%%", m_Answer * 100);
}

// [ITEM] CPU Data Rate

// TODO: make bus randomize bytes/bits to add challenge

void GAME::Question_CompSys_CPUDataRate::GenerateProblem(std::mt19937& rng) {
	m_Bus	= (uint32_t)1 << (rng() % 6 + 3);
	m_Cycle = (uint32_t)1 << (rng() % 4 + 1);
	m_Speed = BitNumInitRandom((uint64_t)(1 << (rng() % 6 + 3)), rng);
	m_Speed.value *= 1000 + rng() % 2 * 1000;
	m_Speed.isBytes = false;

	m_Answer	 = BitNumInitRandom(m_Speed.value / m_Cycle * m_Bus, rng);
	m_Answer.mag = std::min((uint8_t)2, m_Answer.mag);
}

void GAME::Question_CompSys_CPUDataRate::RenderQuestion(
	std::unordered_map<std::string, PLAT::ImguiFont>& fonts
) {
	static ImGuiTableFlags tableFlags
		= ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_NoHostExtendX;
	if (ImGui::BeginTable("Question_CompSys_CPUDataRate_question", 2, tableFlags)) {
		ImGui::TableNextColumn();
		ImGui::Text("Bus width:");
		ImGui::TableNextColumn();
		ImGui::Text("%u bits", m_Bus);
		ImGui::TableNextColumn();
		ImGui::Text("Input clock:");
		ImGui::TableNextColumn();
		ImGui::Text("%s", BitNumGetHz(m_Speed).c_str());
		ImGui::TableNextColumn();
		ImGui::Text("Clocks/cycle:");
		ImGui::TableNextColumn();
		ImGui::Text("%u", m_Cycle);
		char answerUnits[8]{ "" };
		BitNumGetUnit(m_Answer, answerUnits);
		ImGui::TableNextColumn();
		ImGui::Text("ANSWER USING");
		ImGui::TableNextColumn();
		ImGui::Text("%s/s", answerUnits);
		ImGui::EndTable();
	}
}

void GAME::Question_CompSys_CPUDataRate::RenderHint(
	std::unordered_map<std::string, PLAT::ImguiFont>& fonts
) {
	ImGui::PushFont(fonts["FormulaText"].font);
	ImGui::Text("data_rate_second = clock_speed / clocks_per_bus_cycle * bus_width");
	ImGui::PopFont();
}

void GAME::Question_CompSys_CPUDataRate::RenderAnswer(
	std::unordered_map<std::string, PLAT::ImguiFont>& fonts
) {
	ImGui::Text("%s/s", BitNumGetSize(m_Answer).c_str());
}

// [ITEM] CPU MIPS

// TODO: implement randomized prompt units (ns, us, ms)
// original question from past exams asks in nanoseconds tho

void GAME::Question_CompSys_CPU_MIPS_IET::GenerateProblem(std::mt19937& rng) {
	m_AIET	 = rng() % 200 * 5 + 5;
	m_Answer = (float)1000 / m_AIET;
}

void GAME::Question_CompSys_CPU_MIPS_IET::RenderQuestion(
	std::unordered_map<std::string, PLAT::ImguiFont>& fonts
) {
	static ImGuiTableFlags tableFlags
		= ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_NoHostExtendX;
	if (ImGui::BeginTable("Question_CompSys_CPU_MIPS_IET_question", 2, tableFlags)) {
		ImGui::TableNextColumn();
		ImGui::Text("Avg. instruction execution time:");
		ImGui::TableNextColumn();
		ImGui::Text("%uns", m_AIET);
		ImGui::EndTable();
	}
}

void GAME::Question_CompSys_CPU_MIPS_IET::RenderHint(
	std::unordered_map<std::string, PLAT::ImguiFont>& fonts
) {
	ImGui::PushFont(fonts["FormulaText"].font);
	ImGui::Text("MIPS = 1000 / avg_execution_time_ns");
	ImGui::PopFont();
}

void GAME::Question_CompSys_CPU_MIPS_IET::RenderAnswer(
	std::unordered_map<std::string, PLAT::ImguiFont>& fonts
) {
	ImGui::Text("%.3f MIPS", m_Answer);
}

// [ITEM] HDD Sector Size

void GAME::Question_CompSys_HDD_SectorSize::GenerateProblem(std::mt19937& rng) {
	m_Plat		   = (uint16_t)1 << (rng() % 3 + 1);
	m_Surf		   = (uint16_t)1 << (rng() % 3 + 1);
	m_Trak		   = (uint32_t)(1 << (rng() % 5 + 1)) * 100;
	m_Sect		   = (uint32_t)(1 << (rng() % 5 + 1)) * 10;
	m_Capacity	   = BitNumInitRandom((uint64_t)(1 << (rng() % 6 + 3)) * 8000000000, rng);
	m_Capacity.mag = rng() % 3 + 1;
	m_Answer = BitNumInitRandom(m_Capacity.value / (m_Plat * m_Surf * m_Trak * m_Sect), rng);
}

void GAME::Question_CompSys_HDD_SectorSize::RenderQuestion(
	std::unordered_map<std::string, PLAT::ImguiFont>& fonts
) {
	static ImGuiTableFlags tableFlags
		= ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_NoHostExtendX;
	if (ImGui::BeginTable("Question_CompSys_HDD_SectorSize_question", 2, tableFlags)) {
		ImGui::TableNextColumn();
		ImGui::Text("Capacity:");
		ImGui::TableNextColumn();
		ImGui::Text("%s", BitNumGetSize(m_Capacity).c_str());
		ImGui::TableNextColumn();
		ImGui::Text("Platters:");
		ImGui::TableNextColumn();
		ImGui::Text("%u", m_Plat);
		ImGui::TableNextColumn();
		ImGui::Text("Surfaces:");
		ImGui::TableNextColumn();
		ImGui::Text("%u", m_Surf);
		ImGui::TableNextColumn();
		ImGui::Text("Tracks:");
		ImGui::TableNextColumn();
		ImGui::Text("%u", m_Trak);
		ImGui::TableNextColumn();
		ImGui::Text("Sectors:");
		ImGui::TableNextColumn();
		ImGui::Text("%u", m_Sect);
		char answerUnits[8]{ "" };
		BitNumGetUnit(m_Answer, answerUnits);
		ImGui::TableNextColumn();
		ImGui::Text("ANSWER USING");
		ImGui::TableNextColumn();
		ImGui::Text("%s", answerUnits);
		ImGui::EndTable();
	}
}

void GAME::Question_CompSys_HDD_SectorSize::RenderHint(
	std::unordered_map<std::string, PLAT::ImguiFont>& fonts
) {
	ImGui::Text("Divide the total capacity by all the elements.");
	ImGui::PushFont(fonts["FormulaText"].font);
	ImGui::Text("sector_size = capacity / (platters * surfaces * tracks * sectors)");
	ImGui::PopFont();
}

void GAME::Question_CompSys_HDD_SectorSize::RenderAnswer(
	std::unordered_map<std::string, PLAT::ImguiFont>& fonts
) {
	ImGui::Text("%s", BitNumGetSize(m_Answer).c_str());
}

// ...