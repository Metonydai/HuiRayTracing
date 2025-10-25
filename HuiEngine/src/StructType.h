#pragma once

#include <glm/glm.hpp>
#include <vulkan/vulkan.hpp>

struct Vertex {

    glm::vec3 position;
    float uv_x;
    glm::vec3 normal;
    float uv_y;
    glm::vec4 color;

    bool operator==(const Vertex& other) const {
        return position == other.position && color == other.color && uv_x == other.uv_x && uv_y == other.uv_y;
    }
};

struct AllocatedBuffer 
{
    VkBuffer buffer;
    VkDeviceMemory memory;
};

// holds the resources needed for a mesh
struct GPUMeshBuffers 
{
    AllocatedBuffer indexBuffer;
    AllocatedBuffer vertexBuffer;
};