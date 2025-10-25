#version 450

layout (push_constant) uniform PushConstant {
    mat4 viewProjection;
    float scale;
} pc;

layout (location = 0) in vec3 inPosition;

void main() 
{
    mat4 scaleMat = mat4(
        vec4(pc.scale, 0.0, 0.0, 0.0),
        vec4(0.0, pc.scale, 0.0, 0.0),
        vec4(0.0, 0.0, pc.scale, 0.0),
        vec4(0.0, 0.0, 0.0, 1.0)
    );

    gl_Position = pc.viewProjection * scaleMat * vec4(inPosition, 1.f);
}
