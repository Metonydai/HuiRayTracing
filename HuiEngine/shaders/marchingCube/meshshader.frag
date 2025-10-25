#version 450
 
layout (location = 0) in VertexInput {
  vec4 color;
  vec3 normal;
} vertexInput;

layout(location = 0) out vec4 outFragColor;
 
vec3 lightDir = vec3(0, 0, 1);

float ambient = 0.2;

void main()
{
	outFragColor = vertexInput.color * (ambient + max(0.0, dot(vertexInput.normal, lightDir)));
}