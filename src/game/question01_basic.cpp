#include "question01_basic.h"

// [SECTION] Basic Theory

// ...

// [SECTION] Algorithms & Programming

#if 0
// [ITEM] Reverse Polish Notation - Solve

void GAME::Question_Basic_RPNSolve::GenerateProblem(std::mt19937& rng) {
	m_Answer		  = {};
	m_ProblemQueue[0] = '+';
	m_ProblemText	  = {};
}
void GAME::Question_Basic_RPNSolve::RenderQuestion(
	std::unordered_map<std::string, PLAT::ImguiFont>& fonts
) {
	ImGui::Text("RenderQuestion1");
	ImGui::Text("RenderQuestion2");
}
void GAME::Question_Basic_RPNSolve::RenderHint(
	std::unordered_map<std::string, PLAT::ImguiFont>& fonts
) {
	ImGui::Text("RenderHint1");
	ImGui::Text("RenderHint2");
}
void GAME::Question_Basic_RPNSolve::RenderAnswer(
	std::unordered_map<std::string, PLAT::ImguiFont>& fonts
) {
	ImGui::Text("RenderAnswer1");
	ImGui::Text("RenderAnswer2");
}

// [ITEM] Reverse Polish Notation - Annotate

void GAME::Question_Basic_RPNInterpret::GenerateProblem(std::mt19937& rng) {
	m_ProblemQueue[0] = '+';
	m_ProblemText	  = {};
}
void GAME::Question_Basic_RPNInterpret::RenderQuestion(
	std::unordered_map<std::string, PLAT::ImguiFont>& fonts
) {
	ImGui::Text("RenderQuestion1");
	ImGui::Text("RenderQuestion2");
}
void GAME::Question_Basic_RPNInterpret::RenderHint(
	std::unordered_map<std::string, PLAT::ImguiFont>& fonts
) {
	ImGui::Text("RenderHint1");
	ImGui::Text("RenderHint2");
}
void GAME::Question_Basic_RPNInterpret::RenderAnswer(
	std::unordered_map<std::string, PLAT::ImguiFont>& fonts
) {
	ImGui::Text("RenderAnswer1");
	ImGui::Text("RenderAnswer2");
}
#endif

// [ITEM] PROBABILITY - at least N heads in X coin tosses

void GAME::Question_Basic_ProbCoinTossMinHeads::GenerateProblem(std::mt19937& rng) {
	m_Tosses   = rng() % 5 + 4;
	m_MinHeads = rng() % 4 + 1;
	uint32_t success{ 0 };
	for (uint32_t i = m_MinHeads; i <= m_Tosses; i++)
		success += Fact(m_Tosses) / Fact(m_Tosses - i) / Fact(i);
	m_Answer	   = (float)success / (1 << m_Tosses);
}

void GAME::Question_Basic_ProbCoinTossMinHeads::RenderQuestion(
	std::unordered_map<std::string, PLAT::ImguiFont>& fonts
) {
	static ImGuiTableFlags tableFlags
		= ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_NoHostExtendX;
	if (ImGui::BeginTable("Basic_ProbCoinTossMinHeads_question", 2, tableFlags)) {
		ImGui::TableNextColumn();
		ImGui::Text("Coin tosses");
		ImGui::TableNextColumn();
		ImGui::Text("%u", m_Tosses);
		ImGui::TableNextColumn();
		ImGui::Text("Min. Heads");
		ImGui::TableNextColumn();
		ImGui::Text("%u", m_MinHeads);
		ImGui::EndTable();
	}
}

void GAME::Question_Basic_ProbCoinTossMinHeads::RenderHint(
	std::unordered_map<std::string, PLAT::ImguiFont>& fonts
) {
	ImGui::Text("Ratio of the total successes to the total trials.");
	ImGui::PushFont(fonts["FormulaText"].font);
	ImGui::Text("probability = sum(cases(N2=N+1)..cases(N2=X)) / total_trials");
	ImGui::PopFont();
	ImGui::Text("Number of cases formula (occurrences of N outcomes in X trials)");
	ImGui::PushFont(fonts["FormulaText"].font);
	ImGui::Text("total = X! / N2! / N!");
	ImGui::PopFont();
}

void GAME::Question_Basic_ProbCoinTossMinHeads::RenderAnswer(
	std::unordered_map<std::string, PLAT::ImguiFont>& fonts
) {
	ImGui::Text("%0.5f", m_Answer);
}

// [ITEM] Sorting - Sort Base Class

void GAME::Question_Basic_SortBASE::GenerateProblem(std::mt19937& rng) {
	m_Data = SortInit(rng);
}

void GAME::Question_Basic_SortBASE::RenderQuestion(
	std::unordered_map<std::string, PLAT::ImguiFont>& fonts
) {
	static ImGuiTableFlags tableFlags = ImGuiTableFlags_SizingFixedFit
									  | ImGuiTableFlags_NoHostExtendX | ImGuiTableFlags_Borders;
	if (ImGui::BeginTable("Basic_SortBASE_question", 8, tableFlags)) {
		ImGui::PushFont(fonts["FormulaText"].font);
		for (auto& i : m_Data.data) {
			ImGui::TableNextColumn();
			ImGui::Text("%02u", i);
		}
		ImGui::PopFont();
		ImGui::EndTable();
	}
}

void GAME::Question_Basic_SortBASE::RenderHint(
	std::unordered_map<std::string, PLAT::ImguiFont>& fonts
) {
	ImGui::Text("You're on your own, kid.");
}

void GAME::Question_Basic_SortBASE::RenderAnswer(
	std::unordered_map<std::string, PLAT::ImguiFont>& fonts
) {
	ImGui::Text("%u loop iterations, %u swap operations", m_Data.loopCount, m_Data.swapCount);
}

// [ITEM] Sorting - Random algorithm

void GAME::Question_Basic_SortRandomAlgo::GenerateProblem(std::mt19937& rng) {
	m_Data = SortInit(rng);
	switch (rng() % 8) {
	case 0:
		m_Algo = "Bubble Sort";
		SortCountBubble(m_Data);
		break;
	case 1:
		m_Algo = "Shaker Sort";
		SortCountShaker(m_Data);
		break;
	case 2:
		m_Algo = "Selection Sort";
		SortCountSelection(m_Data);
		break;
	case 3:
		m_Algo = "Heapsort (not implemented, reroll)";
		SortCountHeapsort(m_Data);
		break;
	case 4:
		m_Algo = "Insertion Sort";
		SortCountInsertion(m_Data);
		break;
	case 5:
		m_Algo = "Shell Sort";
		SortCountShell(m_Data);
		break;
	case 6:
		m_Algo = "Quicksort";
		SortCountQuicksort(m_Data);
		break;
	case 7:
		m_Algo = "Merge Sort";
		SortCountMerge(m_Data);
		break;

	default:
		break;
	}
}

void GAME::Question_Basic_SortRandomAlgo::RenderQuestion(
	std::unordered_map<std::string, PLAT::ImguiFont>& fonts
) {
	ImGui::Text("Algorithm: %s", m_Algo);
	static ImGuiTableFlags tableFlags = ImGuiTableFlags_SizingFixedFit
									  | ImGuiTableFlags_NoHostExtendX | ImGuiTableFlags_Borders;
	if (ImGui::BeginTable("Basic_SortRandomAlgo_question", 8, tableFlags)) {
		ImGui::PushFont(fonts["FormulaText"].font);
		for (auto& i : m_Data.data) {
			ImGui::TableNextColumn();
			ImGui::Text("%02u", i);
		}
		ImGui::PopFont();
		ImGui::EndTable();
	}
}

// [ITEM] Sorting - Bubble Sort

void GAME::Question_Basic_SortBubble::GenerateProblem(std::mt19937& rng) {
	m_Data = SortInit(rng);
	SortCountBubble(m_Data);
}

void GAME::Question_Basic_SortBubble::RenderHint(
	std::unordered_map<std::string, PLAT::ImguiFont>& fonts
) {
	ImGui::Text("Iterate over the array, swapping the current and next values if current is "
				"larger than next.");
	ImGui::Text("Stop when a loop has no swaps.");
}

// [ITEM] Sorting - Shaker Sort

void GAME::Question_Basic_SortShaker::GenerateProblem(std::mt19937& rng) {
	m_Data = SortInit(rng);
	SortCountShaker(m_Data);
}

void GAME::Question_Basic_SortShaker::RenderHint(
	std::unordered_map<std::string, PLAT::ImguiFont>& fonts
) {
	ImGui::Text("Same as Bubble Sort, but swap direction at the end of an iteration instead of "
				"going back to the start.");
}

// [ITEM] Sorting - Selection Sort

void GAME::Question_Basic_SortSelection::GenerateProblem(std::mt19937& rng) {
	m_Data = SortInit(rng);
	SortCountSelection(m_Data);
}

void GAME::Question_Basic_SortSelection::RenderHint(
	std::unordered_map<std::string, PLAT::ImguiFont>& fonts
) {
	ImGui::Text("Keep track of the number of sorted elements.");
	ImGui::Text("Iterate over the array to find the smallest value, then swap it with the "
				"leftmost unsorted element.");
	ImGui::Text("Exit when all elements are sorted or there are no swaps in a loop.");
}

// [ITEM] Sorting - Heap Sort

void GAME::Question_Basic_SortHeapsort::GenerateProblem(std::mt19937& rng) {
	m_Data = SortInit(rng);
	SortCountHeapsort(m_Data);
}

void GAME::Question_Basic_SortHeapsort::RenderHint(
	std::unordered_map<std::string, PLAT::ImguiFont>& fonts
) {
	ImGui::Text("...");
}

// [ITEM] Sorting - Insertion Sort

void GAME::Question_Basic_SortInsertion::GenerateProblem(std::mt19937& rng) {
	m_Data = SortInit(rng);
	SortCountInsertion(m_Data);
}

void GAME::Question_Basic_SortInsertion::RenderHint(
	std::unordered_map<std::string, PLAT::ImguiFont>& fonts
) {
	ImGui::Text("Iterate over the array until i>i+1");
	ImGui::Text("Move the value at i+1 backward as long as it's smaller than the left value.");
	ImGui::Text("Return to step 1 and continue until the main iterator reaches the end.");
}

// [ITEM] Sorting - Shell Sort

void GAME::Question_Basic_SortShell::GenerateProblem(std::mt19937& rng) {
	m_Data = SortInit(rng);
	SortCountShell(m_Data);
}

void GAME::Question_Basic_SortShell::RenderHint(
	std::unordered_map<std::string, PLAT::ImguiFont>& fonts
) {
	ImGui::Text("Same as Insertion Sort, but input is broken up into N interleaved arrays.");
	ImGui::Text("All the subarrays are insertion-sorted individually, then N is decremented.");
	ImGui::Text("Repeat from step 1 until an N=1 pass is complete.");
	ImGui::Text("To find starting N, use the highest Knuth sequence (n=4n+1) value that is "
				"less than the array length.");
	ImGui::Text("To decrement N, go one Knuth sequence value smaller (n=(n-1)/4).");
}

// [ITEM] Sorting - Quick Sort

void GAME::Question_Basic_SortQuicksort::GenerateProblem(std::mt19937& rng) {
	m_Data = SortInit(rng);
	SortCountQuicksort(m_Data);
}

void GAME::Question_Basic_SortQuicksort::RenderHint(
	std::unordered_map<std::string, PLAT::ImguiFont>& fonts
) {
	ImGui::Text("Select a 'pivot' point and move all lower values to its left and higher "
				"values to its right.");
	ImGui::Text("Do the same for each subarray on either side of the pivot.");
	ImGui::Text("Repeat the above step on each subarray recursively until there is no more "
				"sorting to be done.");
	ImGui::Text("Sedgewick: pivot is the median of the first, middle and last value.");
	ImGui::Text("Other common partitioning schemes are Lomuto and Hoare.");
}

// [ITEM] Sorting - Merge Sort

void GAME::Question_Basic_SortMerge::GenerateProblem(std::mt19937& rng) {
	m_Data = SortInit(rng);
	SortCountMerge(m_Data);
}

void GAME::Question_Basic_SortMerge::RenderHint(
	std::unordered_map<std::string, PLAT::ImguiFont>& fonts
) {
	ImGui::Text("Split the array in half recursively until there is <=1 item on both sides.");
	ImGui::Text("Merge (interleave such that smaller values get folded in in order) both sides "
				"of the subarray.");
	ImGui::Text(
		"Move up a recursion layer and repeat last step until all layers have been merged."
	);
	ImGui::Text(
		"Splitting down to size=2 means they don't need to be sorted to fold in order, meaning "
		"every level above the bottom is guaranteed to have two sorted halves to merge."
	);
	ImGui::Text("...");
	ImGui::Text("...");
}

// [ITEM] Hamming Distance

void GAME::Question_Basic_HammingDistance::GenerateProblem(std::mt19937& rng) {
	m_Answer	 = 0;
	m_BitString1 = 0;
	m_BitString2 = 0;

	bool a, b;
	for (int i = 0; i < 32; ++i) {
		if ((a = (rng() % 2) == 0))
			m_BitString1 |= (uint32_t)1 << i;
		if ((b = (rng() % 2) == 0))
			m_BitString2 |= (uint32_t)1 << i;
		if (a != b)
			m_Answer++;
	}
}

void GAME::Question_Basic_HammingDistance::RenderQuestion(
	std::unordered_map<std::string, PLAT::ImguiFont>& fonts
) {
	static ImGuiTableFlags tableFlags
		= ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_NoHostExtendX;
	if (ImGui::BeginTable("Basic_HammingDistance_question", 2, tableFlags)) {
		ImGui::TableNextColumn();
		ImGui::Text("Bit String 1");
		ImGui::TableNextColumn();
		ImGui::PushFont(fonts["FormulaText"].font);
		ImGui::Text("%s", BitFNum(m_BitString1).c_str());
		ImGui::PopFont();
		ImGui::TableNextColumn();
		ImGui::Text("Bit String 2");
		ImGui::TableNextColumn();
		ImGui::PushFont(fonts["FormulaText"].font);
		ImGui::Text("%s", BitFNum(m_BitString2).c_str());
		ImGui::PopFont();
		ImGui::EndTable();
	}
}

void GAME::Question_Basic_HammingDistance::RenderHint(
	std::unordered_map<std::string, PLAT::ImguiFont>& fonts
) {
	ImGui::Text("Number of changes required to make the strings match.");
}

void GAME::Question_Basic_HammingDistance::RenderAnswer(
	std::unordered_map<std::string, PLAT::ImguiFont>& fonts
) {
	ImGui::Text("%i", m_Answer);
}

// [ITEM] Hamming Weight

void GAME::Question_Basic_HammingWeight::GenerateProblem(std::mt19937& rng) {
	m_Answer	= 0;
	m_BitString = 0;

	bool a;
	for (int i = 0; i < 32; ++i) {
		if ((a = (rng() % 2) == 0)) {
			m_BitString |= (uint32_t)1 << i;
			m_Answer++;
		}
	}
}

void GAME::Question_Basic_HammingWeight::RenderQuestion(
	std::unordered_map<std::string, PLAT::ImguiFont>& fonts
) {
	static ImGuiTableFlags tableFlags
		= ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_NoHostExtendX;
	if (ImGui::BeginTable("Basic_HammingWeight_question", 2, tableFlags)) {
		ImGui::TableNextColumn();
		ImGui::Text("Bit String");
		ImGui::TableNextColumn();
		ImGui::PushFont(fonts["FormulaText"].font);
		ImGui::Text("%s", BitFNum(m_BitString).c_str());
		ImGui::PopFont();
		ImGui::EndTable();
	}
}

void GAME::Question_Basic_HammingWeight::RenderHint(
	std::unordered_map<std::string, PLAT::ImguiFont>& fonts
) {
	ImGui::Text("Number of symbols that are different from the zero-value of a string.");
}

void GAME::Question_Basic_HammingWeight::RenderAnswer(
	std::unordered_map<std::string, PLAT::ImguiFont>& fonts
) {
	ImGui::Text("%i", m_Answer);
}

// [ITEM] Hamming Code

void GAME::Question_Basic_HammingCode::GenerateProblem(std::mt19937& rng) {
	m_RedundancySize = (rng() % 14) + 2;
	m_DataSize		 = (uint32_t)1 << m_RedundancySize;
	m_BlockSize		 = m_DataSize - 1;
	m_MessageSize	 = m_BlockSize - m_RedundancySize;
}

void GAME::Question_Basic_HammingCode::RenderQuestion(
	std::unordered_map<std::string, PLAT::ImguiFont>& fonts
) {
	static ImGuiTableFlags tableFlags
		= ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_NoHostExtendX;
	if (ImGui::BeginTable("Basic_HammingCode_question", 2, tableFlags)) {
		ImGui::TableNextColumn();
		ImGui::Text("Data Size");
		ImGui::TableNextColumn();
		ImGui::Text("%u bits", m_DataSize);
		ImGui::EndTable();
	}
}

void GAME::Question_Basic_HammingCode::RenderHint(
	std::unordered_map<std::string, PLAT::ImguiFont>& fonts
) {
	ImGui::PushFont(fonts["FormulaText"].font);
	ImGui::Text("block_size = data_size - 1");
	ImGui::Text("message_size = block_size - log2(data_size)");
	ImGui::PopFont();
}

void GAME::Question_Basic_HammingCode::RenderAnswer(
	std::unordered_map<std::string, PLAT::ImguiFont>& fonts
) {
	ImGui::Text("Hamming(%u, %u)", m_BlockSize, m_MessageSize);
}

// [ITEM] Find Error in Hamming-encoded data

void GAME::Question_Basic_HammingError::GenerateProblem(std::mt19937& rng) {
	m_Data = 0;
	for (uint8_t i = 0; i < 16; ++i)
		if (rng() % 2 == 0)
			m_Data |= (uint16_t)1 << i;
	m_Data &= 0b1111111011101000;

	for (uint8_t i = 0; i < 4; ++i) {
		uint8_t pos	  = (uint8_t)1 << i;
		uint8_t count = 0;
		for (uint16_t j = pos; j < 16; ++j)
			if ((pos & j) && (m_Data & ((uint16_t)1 << j)))
				count++;
		if (count % 2 == 1)
			m_Data |= (uint16_t)1 << pos;
	}

	uint8_t count = 0;
	for (uint16_t i = m_Data; i > 0; i = (i >> 1))
		if (i & (uint16_t)1)
			count++;
	if (count % 2 == 1)
		m_Data |= (uint16_t)1;

	m_ErrorCount = 0;
	m_ErrorPos	 = -1;
	int16_t pos	 = -1;
	for (uint8_t i = 0; i < 2; ++i) {
		if (rng() % 2) {
			m_ErrorCount++;
			do
				pos = rng() % 16;
			while (pos == m_ErrorPos);
			m_ErrorPos = m_ErrorCount == 1 ? pos : -1;
			m_Data ^= (uint16_t)1 << pos;
		}
	}

	m_DataRev = 0;
	for (uint8_t i = 0; i < 16; ++i)
		if ((((uint16_t)1 << i) & m_Data) > 0)
			m_DataRev |= (uint16_t)0x8000 >> i;
}

void GAME::Question_Basic_HammingError::RenderQuestion(
	std::unordered_map<std::string, PLAT::ImguiFont>& fonts
) {
	static ImGuiTableFlags tableFlags
		= ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_NoHostExtendX;
	if (ImGui::BeginTable("Basic_HammingError_question", 2, tableFlags)) {
		ImGui::TableNextColumn();
		ImGui::Text("Encoded Data Sequence");
		ImGui::TableNextColumn();
		ImGui::PushFont(fonts["FormulaText"].font);
		ImGui::Text("%s", BitFNum(m_DataRev).c_str());
		ImGui::PopFont();
		ImGui::EndTable();
	}
}

void GAME::Question_Basic_HammingError::RenderHint(
	std::unordered_map<std::string, PLAT::ImguiFont>& fonts
) {
	// TODO: draw table with colored parity cells for illustration
	static ImGuiTableFlags tableFlags = ImGuiTableFlags_SizingFixedFit
									  | ImGuiTableFlags_NoHostExtendX | ImGuiTableFlags_Borders;

	ImGui::PushFont(fonts["FormulaText"].font);
	if (ImGui::BeginTable("Basic_HammingError_hint", 4, tableFlags)) {
		uint32_t x = m_DataRev;
		for (int i = 0; i < 16; ++i) {
			ImGui::TableNextColumn();
			ImGui::Text("%i", (((uint32_t)0x8000 >> i) & x) > 0 ? 1 : 0);
		}
		ImGui::EndTable();
	}
	ImGui::PopFont();
	ImGui::Text("1+log2(size) parity bits total.");
	ImGui::Text("1st parity bit helps determine if there are either 0 or 2 errors.");
	ImGui::Text("Other parity bits act as a binary search for the error location in the case "
				"of 1 error.");
}

void GAME::Question_Basic_HammingError::RenderAnswer(
	std::unordered_map<std::string, PLAT::ImguiFont>& fonts
) {
	switch (m_ErrorCount) {
	case 1:
		ImGui::Text("1 error at position %d", m_ErrorPos);
		break;
	default:
		ImGui::Text("%u errors", m_ErrorCount);
		break;
	}
}
