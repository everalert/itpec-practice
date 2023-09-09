#pragma once

#include "question.h"

namespace GAME {

// [SECTION] Basic Theory

// ...

// [SECTION] Algorithms & Programming

#if 0
// [ITEM] Reverse Polish Notation - Solve

class Question_Basic_RPNSolve : public Q {
public:
	Question_Basic_RPNSolve() {
		Q::strVerb	 = "CALC";
		Q::strResult = "Solution";
		Q::strMethod = "Reverse Polish Notation";
		Q::strBookPg = "IT Fundamentals, pp.221,412";
	};
	void GenerateProblem(std::mt19937& rng);
	void RenderQuestion(std::unordered_map<std::string, PLAT::ImguiFont>& fonts);
	void RenderHint(std::unordered_map<std::string, PLAT::ImguiFont>& fonts);
	void RenderAnswer(std::unordered_map<std::string, PLAT::ImguiFont>& fonts);

private:
	int32_t m_Answer;
	std::string m_ProblemText;
	char m_ProblemQueue[15]{}; // treat everything other than +,-,x,/,0 as int8
};

// [ITEM] Reverse Polish Notation - Annotate

class Question_Basic_RPNInterpret : public Q {
public:
	Question_Basic_RPNInterpret() {
		Q::strVerb	 = "READ";
		Q::strResult = "Structure";
		Q::strMethod = "Reverse Polish Notation";
		Q::strBookPg = "IT Fundamentals, pp.221,412";
	};
	void GenerateProblem(std::mt19937& rng);
	void RenderQuestion(std::unordered_map<std::string, PLAT::ImguiFont>& fonts);
	void RenderHint(std::unordered_map<std::string, PLAT::ImguiFont>& fonts);
	void RenderAnswer(std::unordered_map<std::string, PLAT::ImguiFont>& fonts);

private:
	std::string m_ProblemText;
	char m_ProblemQueue[15]{}; // treat everything other than +,-,x,/,0 as int8
};
#endif

// [ITEM] PROBABILITY - at least N heads in X coin tosses

class Question_Basic_ProbCoinTossMinHeads : public Q {
public:
	Question_Basic_ProbCoinTossMinHeads() {
		Q::strVerb	 = "CALC";
		Q::strResult = "Probability of at least N heads in X coin tosses, in decimal";
	}
	void GenerateProblem(std::mt19937& rng);
	void RenderQuestion(std::unordered_map<std::string, PLAT::ImguiFont>& fonts);
	void RenderHint(std::unordered_map<std::string, PLAT::ImguiFont>& fonts);
	void RenderAnswer(std::unordered_map<std::string, PLAT::ImguiFont>& fonts);

	float m_Answer;
	uint8_t m_MinHeads;
	uint8_t m_Tosses;
};

// [ITEM] SORTING - Bubble Sort

class Question_Basic_SortBASE : public Q {
public:
	Question_Basic_SortBASE() {
		Q::strVerb	 = "CALC";
		Q::strResult = "Count sorting swaps and loop iterations";
	}
	void GenerateProblem(std::mt19937& rng);
	void RenderQuestion(std::unordered_map<std::string, PLAT::ImguiFont>& fonts);
	void RenderHint(std::unordered_map<std::string, PLAT::ImguiFont>& fonts);
	void RenderAnswer(std::unordered_map<std::string, PLAT::ImguiFont>& fonts);

	SortingData m_Data;
};

// [ITEM] SORTING - RANDOM ALGORITHM

class Question_Basic_SortRandomAlgo : public Question_Basic_SortBASE {
public:
	Question_Basic_SortRandomAlgo() {
		Q::strResult = "Count sorting swaps and loop iterations";
	}
	void GenerateProblem(std::mt19937& rng);
	void RenderQuestion(std::unordered_map<std::string, PLAT::ImguiFont>& fonts);
	const char* m_Algo;
};

// [ITEM] SORTING - Bubble Sort

class Question_Basic_SortBubble : public Question_Basic_SortBASE {
public:
	Question_Basic_SortBubble() {
		Q::strResult = "Bubble Sort - Count swaps and loop iterations";
	}
	void GenerateProblem(std::mt19937& rng);
	void RenderHint(std::unordered_map<std::string, PLAT::ImguiFont>& fonts);
};

// [ITEM] SORTING - Shaker Sort

class Question_Basic_SortShaker : public Question_Basic_SortBASE {
public:
	Question_Basic_SortShaker() {
		Q::strResult = "Shaker Sort - Count swaps and loop iterations";
	}
	void GenerateProblem(std::mt19937& rng);
	void RenderHint(std::unordered_map<std::string, PLAT::ImguiFont>& fonts);
};

// [ITEM] SORTING - Selection Sort

class Question_Basic_SortSelection : public Question_Basic_SortBASE {
public:
	Question_Basic_SortSelection() {
		Q::strResult = "Selection Sort - Count swaps and loop iterations";
	}
	void GenerateProblem(std::mt19937& rng);
	void RenderHint(std::unordered_map<std::string, PLAT::ImguiFont>& fonts);
};

// [ITEM] SORTING - Heap Sort

class Question_Basic_SortHeapsort : public Question_Basic_SortBASE {
public:
	Question_Basic_SortHeapsort() {
		Q::strResult = "Heapsort - not implemented, just go next.";
		// Q::strResult = "Heapsort - Count swaps and loop iterations";
	}
	void GenerateProblem(std::mt19937& rng);
	void RenderHint(std::unordered_map<std::string, PLAT::ImguiFont>& fonts);
};

// [ITEM] SORTING - Insertion Sort

class Question_Basic_SortInsertion : public Question_Basic_SortBASE {
public:
	Question_Basic_SortInsertion() {
		Q::strResult = "Insertion Sort - Count swaps and loop iterations";
	}
	void GenerateProblem(std::mt19937& rng);
	void RenderHint(std::unordered_map<std::string, PLAT::ImguiFont>& fonts);
};

// [ITEM] SORTING - Shell Sort

class Question_Basic_SortShell : public Question_Basic_SortBASE {
public:
	Question_Basic_SortShell() {
		Q::strResult = "Shell Sort - Count swaps and loop iterations, use Knuth sequence";
	}
	void GenerateProblem(std::mt19937& rng);
	void RenderHint(std::unordered_map<std::string, PLAT::ImguiFont>& fonts);
};

// [ITEM] SORTING - Quick Sort

class Question_Basic_SortQuicksort : public Question_Basic_SortBASE {
public:
	Question_Basic_SortQuicksort() {
		Q::strResult
			= "Quicksort - Count swaps and recursion iterations, use Sedgewick partitioning";
	}
	void GenerateProblem(std::mt19937& rng);
	void RenderHint(std::unordered_map<std::string, PLAT::ImguiFont>& fonts);
};

// [ITEM] SORTING - Merge Sort

class Question_Basic_SortMerge : public Question_Basic_SortBASE {
public:
	Question_Basic_SortMerge() {
		Q::strResult = "Merge Sort - Count moves and recursion iterations";
	}
	void GenerateProblem(std::mt19937& rng);
	void RenderHint(std::unordered_map<std::string, PLAT::ImguiFont>& fonts);
};

// [ITEM] Hamming Distance

class Question_Basic_HammingDistance : public Q {
public:
	Question_Basic_HammingDistance() {
		Q::strVerb	 = "CALC";
		Q::strResult = "Hamming distance";
		Q::strMethod = "";
		Q::strBookPg = "";
	}
	void GenerateProblem(std::mt19937& rng);
	void RenderQuestion(std::unordered_map<std::string, PLAT::ImguiFont>& fonts);
	void RenderHint(std::unordered_map<std::string, PLAT::ImguiFont>& fonts);
	void RenderAnswer(std::unordered_map<std::string, PLAT::ImguiFont>& fonts);

private:
	int32_t m_Answer;
	uint32_t m_BitString1;
	uint32_t m_BitString2;
};

// [ITEM] Hamming Weight

class Question_Basic_HammingWeight : public Q {
public:
	Question_Basic_HammingWeight() {
		Q::strVerb	 = "CALC";
		Q::strResult = "Hamming weight";
		Q::strMethod = "";
		Q::strBookPg = "";
	}
	void GenerateProblem(std::mt19937& rng);
	void RenderQuestion(std::unordered_map<std::string, PLAT::ImguiFont>& fonts);
	void RenderHint(std::unordered_map<std::string, PLAT::ImguiFont>& fonts);
	void RenderAnswer(std::unordered_map<std::string, PLAT::ImguiFont>& fonts);

private:
	int32_t m_Answer;
	uint32_t m_BitString;
};

// [ITEM] Hamming Code

class Question_Basic_HammingCode : public Q {
public:
	Question_Basic_HammingCode() {
		Q::strVerb	 = "CALC";
		Q::strResult = "Hamming code";
		Q::strMethod = "";
		Q::strBookPg = "";
	}
	void GenerateProblem(std::mt19937& rng);
	void RenderQuestion(std::unordered_map<std::string, PLAT::ImguiFont>& fonts);
	void RenderHint(std::unordered_map<std::string, PLAT::ImguiFont>& fonts);
	void RenderAnswer(std::unordered_map<std::string, PLAT::ImguiFont>& fonts);

private:
	uint32_t m_DataSize;
	uint32_t m_BlockSize;
	uint32_t m_MessageSize;
	uint32_t m_RedundancySize;
};

// [ITEM] Find Error in Hamming-encoded data

class Question_Basic_HammingError : public Q {
public:
	Question_Basic_HammingError() {
		Q::strVerb	 = "FIND";
		Q::strResult = "Error bit(s) in hamming-encoded data";
		Q::strMethod = "";
		Q::strBookPg = "";
	}
	void GenerateProblem(std::mt19937& rng);
	void RenderQuestion(std::unordered_map<std::string, PLAT::ImguiFont>& fonts);
	void RenderHint(std::unordered_map<std::string, PLAT::ImguiFont>& fonts);
	void RenderAnswer(std::unordered_map<std::string, PLAT::ImguiFont>& fonts);

private:
	uint16_t m_DataRev;
	uint16_t m_Data;
	uint16_t m_ErrorCount;
	int16_t m_ErrorPos;
};

} // namespace GAME