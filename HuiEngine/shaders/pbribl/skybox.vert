#version 450

layout (location = 0) in vec3 inPos;
layout (location = 1) in float inUVx;
layout (location = 2) in vec3 inNormal;
layout (location = 3) in float inUVy;
layout (location = 4) in vec4 inColor;


layout (binding = 0) uniform UBO 
{
	mat4 projection;
	mat4 view;
} ubo;

layout (location = 0) out vec3 outPos;

void main() 
{
	outPos = inPos;

	mat4 rotView = mat4(mat3(ubo.view)); // remove translation from the view matrix
	vec4 clipPos = ubo.projection * rotView * vec4(inPos.xyz, 1.0);
	gl_Position = clipPos.xyww;
}
