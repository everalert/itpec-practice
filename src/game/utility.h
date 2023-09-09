#pragma once

#include <_incl_global.hpp>

namespace GAME {

struct BitNum {
	uint64_t value;
	uint8_t mag;  // kilo, mega, giga, etc.
	bool isBytes; // kilobits/kilobytes, etc.
};
BitNum BitNumInitRandom(uint64_t value, std::mt19937& rng);
std::string BitNumGetSize(BitNum& bn);
std::string BitNumGetHz(BitNum& bn);
void BitNumGetVal(BitNum& bn, char* out);
void BitNumGetUnit(BitNum& bn, const char* suffix, char* out);
void BitNumGetUnit(BitNum& bn, char* out);

struct SortingData {
	uint8_t data[8];
	uint8_t dataSorted[8];
	uint32_t swapCount;
	uint32_t loopCount;
};
SortingData SortInit(std::mt19937& rng);
void SortReset(SortingData& data, std::mt19937& rng);
void SortCountBubble(SortingData& data);
void SortCountShaker(SortingData& data);
void SortCountSelection(SortingData& data);
void SortCountHeapsort(SortingData& data);
void SortCountInsertion(SortingData& data);
void SortCountShell(SortingData& data);
void SortCountQuicksort(SortingData& data, uint32_t l = 0, uint32_t r = 0xFFFFFFFF);
void SortCountMerge(SortingData& data, uint32_t l = 0, uint32_t r = 0xFFFFFFFF);

std::string CommaFNum(uint32_t n);
std::string BitFNum(uint32_t n);
std::string BitFNum(uint16_t n);

uint32_t Fact(uint8_t n);

} // namespace GAME