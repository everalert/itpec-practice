#version 450

layout(binding = 0) uniform ubo {
	mat4 model;
	mat4 view;
	mat4 projection;
};

layout(push_constant) uniform constants {
    vec4 data;
    mat4 renderMatrix;
} pushConstants;

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inColor;
layout(location = 2) in vec2 inTexCoord;

layout(location = 0) out vec3 fragColor;
layout(location = 1) out vec2 fragTexCoord;

void main() {
    gl_Position = pushConstants.renderMatrix * vec4(inPosition, 1.0);
    fragColor = inColor;
    fragTexCoord = inTexCoord;
}