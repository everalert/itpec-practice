#pragma once

// failed? attempt at lowering build times by consolidating repeatedly included stuff. only
// seemed to knock off ~2s max from a 20s build, doesn't seem to hurt at least and this style
// could be useful for conveniently including custom base types (i8, u32, etc.)

#include <vma/vk_mem_alloc.h>
#include <vulkan/vulkan.h>

#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_vulkan.h>

#define VK_USE_PLATFORM_WIN32_KHR
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/hash.hpp>

#define NOMINMAX // resolve conflict with std::numeric_limits<>::max()
#include <algorithm>
#include <array>
#include <cassert>
#include <chrono>
#include <cmath>
#include <cstdint>
#include <cstdlib>
#include <fstream>
#include <functional>
#include <iostream>
#include <limits>
#include <memory>
#include <optional>
#include <random>
#include <set>
#include <stdexcept>
#include <stdint.h> // same as cstdint?
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>
