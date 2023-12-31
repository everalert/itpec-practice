#include "utility.h"

std::vector<char> PLAT::ReadFile(const std::string& filename) {
	std::ifstream file(filename, std::ios::ate | std::ios::binary);
	if (!file.is_open())
		throw std::runtime_error("Failed to open file!");

	size_t filesize{ (size_t)file.tellg() };
	std::vector<char> buffer(filesize);
	file.seekg(0);
	file.read(buffer.data(), (int32_t)filesize);

	file.close();
	return buffer;
}