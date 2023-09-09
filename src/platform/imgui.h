#pragma once

#include <_incl_global.hpp>

namespace PLAT {

struct ImguiFont {
	ImFont* font{ nullptr };
	const char* path;
	uint32_t size;
};

void ImguiSetupSizes();
void ImguiSetupColors();
void ImguiSetupFonts(std::unordered_map<std::string, PLAT::ImguiFont>& fonts);

} // namespace PLAT