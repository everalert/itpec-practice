#pragma once

#include <platform/imgui.h>

#include <VLK/vulkan.h>

#include <_incl_global.hpp>

namespace VLK {

void InitImgui(Vulkan& vulkan, std::unordered_map<std::string, PLAT::ImguiFont>& fonts);

void CleanupImgui(void* vulkan, uint32_t i);

}