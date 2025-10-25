#version 450
 
layout (location = 0) in vec3 inNormal;
layout (location = 1) in vec2 inUV;
layout (location = 2) in vec4 inColor;

layout(location = 0) out vec4 outFragColor;
 
void main()
{
	outFragColor = inColor;
}