#pragma once

#include "question.h"

namespace GAME {

// [SECTION] Computer Components

// ...

// [SECTION] System Components

// ...

// [SECTION] Software

// [SECTION] Hardware

// [ITEM] Individual System Availability

class Question_CompSys_SysAvailIndividual : public Q {
public:
	Question_CompSys_SysAvailIndividual() {
		Q::strVerb	 = "CALC";
		Q::strResult = "Individual system availability";
	};
	void GenerateProblem(std::mt19937& rng);
	void RenderQuestion(std::unordered_map<std::string, PLAT::ImguiFont>& fonts);
	void RenderHint(std::unordered_map<std::string, PLAT::ImguiFont>& fonts);
	void RenderAnswer(std::unordered_map<std::string, PLAT::ImguiFont>& fonts);

private:
	float m_Answer;
	uint8_t m_UptimeDays;
	uint8_t m_UptimeHoursPerDay;
	uint32_t m_MTBF;
	uint32_t m_MTTR;
};

// [ITEM] Serial System Availability

class Question_CompSys_SysAvailSerial : public Q {
public:
	Question_CompSys_SysAvailSerial() {
		Q::strVerb	 = "CALC";
		Q::strResult = "Serial system availability";
	};
	void GenerateProblem(std::mt19937& rng);
	void RenderQuestion(std::unordered_map<std::string, PLAT::ImguiFont>& fonts);
	void RenderHint(std::unordered_map<std::string, PLAT::ImguiFont>& fonts);
	void RenderAnswer(std::unordered_map<std::string, PLAT::ImguiFont>& fonts);

private:
	float m_Answer;
	uint8_t m_SystemCount;
	float m_Systems[6];
};

// [ITEM] Parallel System Availability

class Question_CompSys_SysAvailParallel : public Q {
public:
	Question_CompSys_SysAvailParallel() {
		Q::strVerb	 = "CALC";
		Q::strResult = "Parallel system availability";
	};
	void GenerateProblem(std::mt19937& rng);
	void RenderQuestion(std::unordered_map<std::string, PLAT::ImguiFont>& fonts);
	void RenderHint(std::unordered_map<std::string, PLAT::ImguiFont>& fonts);
	void RenderAnswer(std::unordered_map<std::string, PLAT::ImguiFont>& fonts);

private:
	float m_Answer;
	uint8_t m_SystemCount;
	float m_Systems[6];
};

// [ITEM] CPU Data Rate

class Question_CompSys_CPUDataRate : public Q {
public:
	Question_CompSys_CPUDataRate() {
		Q::strVerb	 = "CALC";
		Q::strResult = "CPU Data Rate";
		Q::strMethod = "data/second";
	};
	void GenerateProblem(std::mt19937& rng);
	void RenderQuestion(std::unordered_map<std::string, PLAT::ImguiFont>& fonts);
	void RenderHint(std::unordered_map<std::string, PLAT::ImguiFont>& fonts);
	void RenderAnswer(std::unordered_map<std::string, PLAT::ImguiFont>& fonts);

private:
	uint32_t m_Bus;
	uint32_t m_Cycle;
	BitNum m_Speed;
	BitNum m_Answer;
};

// [ITEM] CPU MIPS

class Question_CompSys_CPU_MIPS_IET : public Q {
public:
	Question_CompSys_CPU_MIPS_IET() {
		Q::strVerb	 = "CALC";
		Q::strResult = "CPU MIPS";
		Q::strMethod = "Avg. instruction execution time";
	};
	void GenerateProblem(std::mt19937& rng);
	void RenderQuestion(std::unordered_map<std::string, PLAT::ImguiFont>& fonts);
	void RenderHint(std::unordered_map<std::string, PLAT::ImguiFont>& fonts);
	void RenderAnswer(std::unordered_map<std::string, PLAT::ImguiFont>& fonts);

private:
	uint32_t m_AIET;
	float m_Answer;
};

// [ITEM] HDD Sector Size

class Question_CompSys_HDD_SectorSize : public Q {
public:
	Question_CompSys_HDD_SectorSize() {
		Q::strVerb	 = "CALC";
		Q::strResult = "HDD Sector Size";
	};
	void GenerateProblem(std::mt19937& rng);
	void RenderQuestion(std::unordered_map<std::string, PLAT::ImguiFont>& fonts);
	void RenderHint(std::unordered_map<std::string, PLAT::ImguiFont>& fonts);
	void RenderAnswer(std::unordered_map<std::string, PLAT::ImguiFont>& fonts);

private:
	uint16_t m_Plat;
	uint16_t m_Surf;
	uint32_t m_Trak;
	uint32_t m_Sect;
	BitNum m_Capacity;
	BitNum m_Answer;
};

// ...

} // namespace GAME