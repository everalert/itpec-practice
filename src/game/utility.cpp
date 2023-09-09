#include "utility.h"

// "BIT NUMBER" DATATYPE

GAME::BitNum GAME::BitNumInitRandom(uint64_t value, std::mt19937& rng) {
	BitNum bn{ value, (uint8_t)(rng() % 3), (rng() % 2) > 0 };
	return bn;
}

std::string GAME::BitNumGetSize(GAME::BitNum& bn) {
	char bufNum[256]{ "" }, bufUnit[8]{ "" };
	BitNumGetVal(bn, bufNum);
	BitNumGetUnit(bn, bufUnit);
	return std::string(bufNum) + bufUnit;
}

std::string GAME::BitNumGetHz(GAME::BitNum& bn) {
	char bufNum[256]{ "" }, bufUnit[8]{ "" };
	BitNumGetVal(bn, bufNum);
	BitNumGetUnit(bn, "Hz", bufUnit);
	return std::string(bufNum) + bufUnit;
}

void GAME::BitNumGetVal(GAME::BitNum& bn, char* out) {
	static char format[8]{ "%.6f" };
	static char formatLen = 7;
	float val{ (float)bn.value / pow(1000.0f, (float)bn.mag) / (bn.isBytes ? 8 : 1) };
	int sz{ std::snprintf(nullptr, 0, format, val) };
	std::snprintf(out, sz + 1, format, val);
	for (int i = 1; (out[sz - i] == '0' || out[sz - i] == '.') && i <= formatLen; ++i)
		out[sz - i] = '\0';
}

void GAME::BitNumGetUnit(BitNum& bn, const char* suffix, char* out) {
	static char mag[6]{ " KMGT" };
	if (bn.mag > 0 && bn.mag < 6)
		out[0] = mag[bn.mag];
	int idx = bn.mag > 0 ? 1 : 0;
	memcpy(out + idx, suffix, strlen(suffix) + 1);
}

void GAME::BitNumGetUnit(BitNum& bn, char* out) {
	char bufB[2]{ "" };
	bufB[0] = bn.isBytes ? 'B' : 'b';
	BitNumGetUnit(bn, bufB, out);
}

// SORTING

GAME::SortingData GAME::SortInit(std::mt19937& rng) {
	SortingData d{};
	SortReset(d, rng);
	return d;
}

void GAME::SortReset(SortingData& data, std::mt19937& rng) {
	for (auto& i : data.data)
		i = rng() % 64 + 1;
	memcpy(&data.dataSorted[0], &data.data[0], 8);
	data.loopCount = 0;
	data.swapCount = 0;
}

void GAME::SortCountBubble(SortingData& data) {
	bool noSwaps{ false };
	uint8_t max = sizeof(data.dataSorted) - 1;
	uint8_t a;
	while (!noSwaps) {
		noSwaps = true;
		for (uint32_t i = 0; i < max; ++i) {
			if (data.dataSorted[i] > data.dataSorted[i + 1]) {
				a					   = data.dataSorted[i];
				data.dataSorted[i]	   = data.dataSorted[i + 1];
				data.dataSorted[i + 1] = a;
				noSwaps				   = false;
				data.swapCount++;
			}
		}
		max--;
		data.loopCount++;
	}
}

void GAME::SortCountShaker(SortingData& data) {
	bool noSwaps{ false };
	bool toRight{ true };
	uint8_t i	= 0;
	uint8_t min = 0;
	uint8_t max = sizeof(data.dataSorted) - 1;
	uint8_t a;
	while (true) {
		if (data.dataSorted[i] > data.dataSorted[i + 1]) {
			a					   = data.dataSorted[i];
			data.dataSorted[i]	   = data.dataSorted[i + 1];
			data.dataSorted[i + 1] = a;
			noSwaps				   = false;
			data.swapCount++;
		}

		if ((!toRight && i == min) || (toRight && i == max - 1)) {
			data.loopCount++;
			if (noSwaps)
				break;
			toRight = !toRight;
			min		= toRight ? ++min : min;
			max		= !toRight ? --max : max;
			noSwaps = true;
		}
		i = toRight ? ++i : --i;
	}
}

// FIXME: something wrong with this, not counting the moves/loops correctly?
void GAME::SortCountSelection(SortingData& data) {
	bool noSwaps{ false };
	uint8_t sorted = 0;
	uint8_t min	   = 0;
	uint8_t a;
	while (!noSwaps) {
		noSwaps = true;
		for (uint32_t i = sorted + 1; i < sizeof(data.dataSorted); ++i)
			if (data.dataSorted[i] < data.dataSorted[min])
				min = i;
		if (sorted != min) {
			a						= data.dataSorted[sorted];
			data.dataSorted[sorted] = data.dataSorted[min];
			data.dataSorted[min]	= a;
			noSwaps					= false;
			data.swapCount++;
		}
		sorted++;
		min = sorted;
		data.loopCount++;
	}
}

void GAME::SortCountHeapsort(SortingData& data) {
	// not implementing for now
}

void GAME::SortCountInsertion(SortingData& data) {
	uint8_t a;
	for (uint32_t i = 0; i < sizeof(data.dataSorted) - 1; ++i) {
		if (data.dataSorted[i] > data.dataSorted[i + 1]) {
			for (uint32_t j = i + 1; j > 0; --j) {
				if (data.dataSorted[j - 1] > data.dataSorted[j]) {
					a					   = data.dataSorted[j];
					data.dataSorted[j]	   = data.dataSorted[j - 1];
					data.dataSorted[j - 1] = a;
					data.swapCount++;
				} else
					break;
			}
		}
	}
	data.loopCount++;
}

void GAME::SortCountShell(SortingData& data) {
	uint8_t sz = sizeof(data.dataSorted);
	uint8_t k  = 1; // knuth
	while (k < sz)
		k = k * 4 + 1;

	uint8_t a;
	while (k > 1) {
		k = (k - 1) / 4;
		for (uint8_t x = 0; x < k; ++x) {
			for (uint32_t y = x; y + k < sz; y += k) {
				if (data.dataSorted[y] > data.dataSorted[y + k]) {
					for (uint32_t z = y + k; z > x; z -= k) {
						if (data.dataSorted[z - k] > data.dataSorted[z]) {
							a					   = data.dataSorted[z];
							data.dataSorted[z]	   = data.dataSorted[z - k];
							data.dataSorted[z - k] = a;
							data.swapCount++;
						} else
							break;
					}
				}
			}
		}
		data.loopCount++;
	}
}

void GAME::SortCountQuicksort(SortingData& data, uint32_t l, uint32_t r) {
	auto& arr{ data.dataSorted };
	l = (l < 0) ? 0 : l;
	r = (r > sizeof(arr) - 1) ? sizeof(arr) - 1 : r;
	if (r <= l)
		return;

	data.loopCount++;
	uint32_t lIn = l;
	uint32_t rIn = r;
	uint32_t a;

	uint32_t p = l + (r - l) / 2;
	p		   = arr[l] < arr[r] ? (arr[l] < arr[p] < arr[r] ? (arr[r] < arr[p] ? r : l) : p)
								 : (arr[l] > arr[p] > arr[r] ? (arr[p] > arr[l] ? l : r) : p);
	if (r != p && arr[r] < arr[p]) {
		a	   = arr[p];
		arr[p] = arr[r];
		arr[r] = a;
		data.swapCount++;
	}
	p = r--;
	while (l <= r) {
		while (arr[r] >= arr[p] && r > lIn)
			r--;
		while (arr[l] <= arr[p] && l < rIn)
			l++;
		if (r < l)
			break;
		a	   = arr[r];
		arr[r] = arr[l];
		arr[l] = a;
		data.swapCount++;
	}
	a	   = arr[p];
	arr[p] = arr[l];
	arr[l] = a;
	data.swapCount++;

	p = l;
	SortCountQuicksort(data, lIn, p - 1);
	SortCountQuicksort(data, p + 1, rIn);
}

void GAME::SortCountMerge(SortingData& data, uint32_t l, uint32_t r) {
	auto& arr{ data.dataSorted };
	l = (l < 0) ? 0 : l;
	r = (r > sizeof(arr) - 1) ? sizeof(arr) : r;
	uint32_t len{ r - l };
	if (len <= 1)
		return;

	data.loopCount++;
	uint32_t m = l + len / 2;
	if (len > 2) {
		SortCountMerge(data, l, m);
		SortCountMerge(data, m, r);
	}

	uint8_t sub[sizeof(arr)];
	memcpy(&sub[0], &arr[0], sizeof(arr));
	uint32_t st{ l }, mid{ m }, end{ r };
	while (l < mid || m < end) {
		if (l < mid && (m >= end || sub[l] <= sub[m])) {
			arr[l + m - mid] = sub[l];
			l++;
			if (l + m - mid != l)
				data.swapCount++;
		} else {
			arr[l + m - mid] = sub[m];
			m++;
			if (l + m - mid != m)
				data.swapCount++;
		}
	}
}

// FORMATTING

// FIXME: doesn't render anything in some cases, e.g. when n=8
std::string GAME::CommaFNum(uint32_t n) {
	uint32_t q{ n / 1000 }, r{ n % 1000 };
	char buf[5]{ "" };
	if (q > 0) {
		std::string next{ CommaFNum(q) };
		std::snprintf(&buf[0], 5, ",%03d", r);
		return next + buf;
	} else {
		int sz{ std::snprintf(nullptr, 0, "%d", r) };
		std::snprintf(&buf[0], sz, "%d", r);
		return buf;
	}
}

std::string GAME::BitFNum(uint32_t n) {
	char buf[36]{ "" };
	for (int i = 0; i < 4; ++i) {
		uint8_t b = static_cast<uint8_t>(n >> (i * 8) & 0b11111111);
		int idx	  = 34 - i * 9;
		if (i < 3)
			buf[idx - 8] = ' ';
		for (int j = 0; j < 8; ++j)
			buf[idx - j] = ((uint8_t)1 << j & b) > 0 ? '1' : '0';
	}
	return buf;
}

std::string GAME::BitFNum(uint16_t n) {
	char buf[18]{ "" };
	for (int i = 0; i < 2; ++i) {
		uint8_t b = static_cast<uint8_t>(n >> (i * 8) & 0b11111111);
		int idx	  = 16 - i * 9;
		if (i < 3)
			buf[idx - 8] = ' ';
		for (int j = 0; j < 8; ++j)
			buf[idx - j] = ((uint8_t)1 << j & b) > 0 ? '1' : '0';
	}
	return buf;
}

uint32_t GAME::Fact(uint8_t n) {
	if (n < 2)
		return 1;

	uint32_t f = n, i = n - 1;
	while (i > 1)
		f *= i--;
	return f;
}