#include "types.h"

VkVertexInputBindingDescription VLK::GetVertexBindingDescription() {
	VkVertexInputBindingDescription description{ 0, sizeof(VLK::Vertex),
												 VK_VERTEX_INPUT_RATE_VERTEX };
	return description;
}

std::array<VkVertexInputAttributeDescription, 3> VLK::GetVertexAttributeDescriptions() {
	std::array<VkVertexInputAttributeDescription, 3> descriptions{};
	descriptions[0] = { 0, 0, VK_FORMAT_R32G32B32_SFLOAT, offsetof(VLK::Vertex, pos) };
	descriptions[1] = { 1, 0, VK_FORMAT_R32G32B32_SFLOAT, offsetof(VLK::Vertex, color) };
	descriptions[2] = { 2, 0, VK_FORMAT_R32G32_SFLOAT, offsetof(VLK::Vertex, texCoord) };
	return descriptions;
}

bool VLK::Vertex::operator==(const Vertex& other) const {
	return pos == other.pos && color == other.color && texCoord == other.texCoord;
}