#version 450

layout (location = 0) in vec3 inPosition;
layout (location = 1) in float inUVx;
layout (location = 2) in vec3 inNormal;
layout (location = 3) in float inUVy;
layout (location = 4) in vec4 inColor;

layout (location = 0) out vec3 outPosition;

layout (push_constant) uniform PushConstant
{
    mat4 viewProjection;
} pc;


void main() 
{
    gl_Position = pc.viewProjection * vec4(inPosition, 1.f);
    outPosition = inPosition;
}
