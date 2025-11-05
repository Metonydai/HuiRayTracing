#include "TinyLoader.h"

#include "vk_engine.h"

#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/hash.hpp>

namespace std {
    template<> struct hash<Vertex> {
        size_t operator()(Vertex const& vertex) const {
            return ((hash<glm::vec3>()(vertex.position) ^ (hash<glm::vec3>()(vertex.color) << 1)) >> 1) ^ (hash<glm::vec2>()({ vertex.uv_x, vertex.uv_y }) << 1);
        }
    };
}

#include <unordered_map>

void LoadedObj::clearAll()
{
    VkDevice dv = creator->GetDevice();

    vkDestroyBuffer(dv, meshBuffers.vertexBuffer.buffer, nullptr);
    vkFreeMemory(dv, meshBuffers.vertexBuffer.memory, nullptr);
    vkDestroyBuffer(dv, meshBuffers.indexBuffer.buffer, nullptr);
    vkFreeMemory(dv, meshBuffers.indexBuffer.memory, nullptr);
}

std::shared_ptr<LoadedObj> loadModel(VulkanEngine* engine, std::string_view path)
{
    std::cout << " Loading GLTF: " << path << std::endl;

    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string warn;
    std::string err;

    if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, path.data()))
    {
        throw std::runtime_error(err);
    }

    std::unordered_map<Vertex, uint32_t> uniqueVertices{};

    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;

    std::shared_ptr<LoadedObj> pModel = std::make_shared<LoadedObj>();
    pModel->creator = engine;
    LoadedObj& model = *pModel.get();

    for (const auto& shape : shapes) {
        for (const auto& index : shape.mesh.indices) {
            Vertex vertex{};

            vertex.position = {
                 attrib.vertices[3 * index.vertex_index + 0],
                 attrib.vertices[3 * index.vertex_index + 1],
                 attrib.vertices[3 * index.vertex_index + 2]
            };

            vertex.uv_x = attrib.texcoords[2 * index.texcoord_index + 0];
            vertex.uv_y = attrib.texcoords[2 * index.texcoord_index + 1];

            vertex.normal = {
                 attrib.normals[3 * index.normal_index + 0],
                 attrib.normals[3 * index.normal_index + 1],
                 attrib.normals[3 * index.normal_index + 2]
            };
            
            vertex.color = {
                 attrib.colors[3 * index.vertex_index + 0],
                 attrib.colors[3 * index.vertex_index + 1],
                 attrib.colors[3 * index.vertex_index + 2],
                 1.0f
            };

            //vertices.push_back(vertex);
            //indices.push_back((uint32_t)indices.size());

            if (uniqueVertices.count(vertex) == 0) {
                uniqueVertices[vertex] = static_cast<uint32_t>(vertices.size());
                vertices.push_back(vertex);
            }
            indices.push_back(uniqueVertices[vertex]);
        }
    }

    model.startIndex = 0;
    model.count = (uint32_t)indices.size();
    model.meshBuffers = engine->UploadMesh(indices, vertices);

    return pModel;
}