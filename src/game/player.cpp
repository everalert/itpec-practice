#include "player.h"

#include <game/question01_basic.h>
#include <game/question02_compSys.h>
#include <game/question06_servMan.h>
#include <game/question09_corpLegal.h>

void GAME::PlayerGenerateQuestions(PlayerObject& p) {
	static uint32_t testVolume{ 1 };
	p.questions.clear();
	for (size_t i = 0; i < testVolume; i += 1) {
		// [SECTION] basic theory
		// p.questions.push_back(std::make_unique<Question_Basic_RPNSolve>());
		// p.questions.back()->GenerateProblem(p.rng);
		// p.questions.push_back(std::make_unique<Question_Basic_RPNInterpret>());
		// p.questions.back()->GenerateProblem(p.rng);
		// p.questions.push_back(std::make_unique<Question_Basic_SortRandomAlgo>());
		// p.questions.back()->GenerateProblem(p.rng);
		p.questions.push_back(std::make_unique<Question_Basic_ProbCoinTossMinHeads>());
		p.questions.back()->GenerateProblem(p.rng);
		p.questions.push_back(std::make_unique<Question_Basic_SortBubble>());
		p.questions.back()->GenerateProblem(p.rng);
		p.questions.push_back(std::make_unique<Question_Basic_SortShaker>());
		p.questions.back()->GenerateProblem(p.rng);
		p.questions.push_back(std::make_unique<Question_Basic_SortSelection>());
		p.questions.back()->GenerateProblem(p.rng);
		// p.questions.push_back(std::make_unique<Question_Basic_SortHeapsort>());
		// p.questions.back()->GenerateProblem(p.rng);
		p.questions.push_back(std::make_unique<Question_Basic_SortInsertion>());
		p.questions.back()->GenerateProblem(p.rng);
		p.questions.push_back(std::make_unique<Question_Basic_SortShell>());
		p.questions.back()->GenerateProblem(p.rng);
		p.questions.push_back(std::make_unique<Question_Basic_SortQuicksort>());
		p.questions.back()->GenerateProblem(p.rng);
		p.questions.push_back(std::make_unique<Question_Basic_SortMerge>());
		p.questions.back()->GenerateProblem(p.rng);
		p.questions.push_back(std::make_unique<Question_Basic_HammingError>());
		p.questions.back()->GenerateProblem(p.rng);
		p.questions.push_back(std::make_unique<Question_Basic_HammingCode>());
		p.questions.back()->GenerateProblem(p.rng);
		p.questions.push_back(std::make_unique<Question_Basic_HammingDistance>());
		p.questions.back()->GenerateProblem(p.rng);
		p.questions.push_back(std::make_unique<Question_Basic_HammingWeight>());
		p.questions.back()->GenerateProblem(p.rng);
		// [SECTION] computer systems
		p.questions.push_back(std::make_unique<Question_CompSys_SysAvailIndividual>());
		p.questions.back()->GenerateProblem(p.rng);
		p.questions.push_back(std::make_unique<Question_CompSys_SysAvailSerial>());
		p.questions.back()->GenerateProblem(p.rng);
		p.questions.push_back(std::make_unique<Question_CompSys_SysAvailParallel>());
		p.questions.back()->GenerateProblem(p.rng);
		p.questions.push_back(std::make_unique<Question_CompSys_CPUDataRate>());
		p.questions.back()->GenerateProblem(p.rng);
		p.questions.push_back(std::make_unique<Question_CompSys_CPU_MIPS_IET>());
		p.questions.back()->GenerateProblem(p.rng);
		p.questions.push_back(std::make_unique<Question_CompSys_HDD_SectorSize>());
		p.questions.back()->GenerateProblem(p.rng);
		// [SECTION] technical elements
		// [SECTION] development techniques
		// [SECTION] project management
		// [SECTION] service management
		p.questions.push_back(std::make_unique<Question_ServMan_MTBF>());
		p.questions.back()->GenerateProblem(p.rng);
		// [SECTION] systems strategy
		// [SECTION] management strategy
		// [SECTION] corporate & legal affairs
		p.questions.push_back(std::make_unique<Question_CorpLegal_StraightLine>());
		p.questions.back()->GenerateProblem(p.rng);
		p.questions.push_back(std::make_unique<Question_CorpLegal_FutureExpectedValue>());
		p.questions.back()->GenerateProblem(p.rng);
	}
}