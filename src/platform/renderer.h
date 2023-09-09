#pragma once

// NOTE: intended to be an interface for the graphics backend, to accommodate different
// implementations for vulkan, dx12, etc.

#include <VLK/types.h>
#include <VLK/vulkan.h>
#include <platform/imgui.h>
#include <platform/window.h>

#include <_incl_global.hpp>

namespace PLAT {

struct Renderer {
	VLK::Vulkan backend{};
	std::unordered_map<std::string, PLAT::ImguiFont> fonts{};
};

// INITIALIZATION

void InitRenderer(Renderer&, WindowDef&);
void CleanupRenderer(Renderer&);

void RenderFrame(Renderer&);

// RESOURCE INTERFACE

// TODO: integrate uniform buffer and push const into renderer interface
//       for better external control over render pipeline

// TODO: ability to create a model from arbitrary vertex input
// TODO: helpers to create primitive models (billboards, cubes, etc.)
// TODO: ability to load models that have pre-calculated index buffers
void CreateModel(
	Renderer&, const std::string& identifier, std::vector<VLK::Vertex>& vertices,
	std::vector<uint32_t>& indices
);
void CreatePlane(
	const glm::vec2& size, const glm::vec3& color, std::vector<VLK::Vertex>& vertices,
	std::vector<uint32_t>& indices
);
void CreateTilePlane(
	const std::vector<bool>& tileMap, uint32_t tilesW, uint32_t tilesH,
	const glm::vec2& tileSize, const glm::vec3& color, std::vector<VLK::Vertex>& vertices,
	std::vector<uint32_t>& indices
);
void CreateCube(
	const glm::vec3& size, const glm::vec3& color, std::vector<VLK::Vertex>& vertices,
	std::vector<uint32_t>& indices
);
void LoadOBJ(const std::string&, std::vector<VLK::Vertex>&, std::vector<uint32_t>&);
void LoadModel(Renderer&, const std::string&, const std::string&);
void DeleteModel(Renderer&, const std::string&);
VLK::ModelResource* GetModel(Renderer&, const std::string&);

// TODO: ability to load textures that already have pre-calculated mipmaps
void LoadTexture(Renderer&, const std::string&, const std::string&);
void DeleteTexture(Renderer&, const std::string&);
VLK::TextureResource* GetTexture(Renderer&, const std::string&);

// FIXME: unable to reuse material objects on a different mesh when creating object,
// fails when allocating descriptor sets; need to retest since implementing descriptor cache
VLK::RenderObject CreateRenderObject(
	Renderer&, const std::string& modelIdentifier, const std::string& textureIdentifier,
	const glm::mat4& transform, std::vector<VLK::ShaderDesc>& shaders,
	VkPushConstantRange& pushConst
);
void CleanupRenderObject(Renderer&, VLK::RenderObject&);

// UTILITY

void RendererWaitIdle(Renderer&);

void RendererUpdateCamera(Renderer&, glm::mat4& matrix);

} // namespace PLAT