#version 450

layout (binding = 0) uniform uniformBuffer {
    mat4 viewProjection;
    vec4 spheres[20];
	int sphereCount;
} ubo;

layout (location = 0) in vec3 inPosition;
layout (location = 1) in float inUVx;
layout (location = 2) in vec3 inNormal;
layout (location = 3) in float inUVy;
layout (location = 4) in vec4 inColor;

layout (location = 0) out vec3 outNormal;
layout (location = 1) out vec2 outUV;
layout (location = 2) out vec4 outColor;

layout (push_constant) uniform PushConstant
{
    layout (offset = 20) float scale;
} pc;


void main() 
{
    mat4 scaleMat = mat4(
        vec4(pc.scale, 0.0, 0.0, 0.0),
        vec4(0.0, pc.scale, 0.0, 0.0),
        vec4(0.0, 0.0, pc.scale, 0.0),
        vec4(0.0, 0.0, 0.0, 1.0)
    );

    gl_Position = ubo.viewProjection * scaleMat * vec4(inPosition, 1.f);
    outColor = inColor;
    outColor.a = 0.1;
    outUV = vec2(inUVx, inUVy);
}
