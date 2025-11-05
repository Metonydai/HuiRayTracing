#pragma once

#include "StructType.h"

class VulkanEngine;

class LoadedObj {
public:
    GPUMeshBuffers meshBuffers;
    uint32_t startIndex;
    uint32_t count;

    VulkanEngine* creator;

    ~LoadedObj() { clearAll(); };
private:
    void clearAll();
};

std::shared_ptr<LoadedObj> loadModel(VulkanEngine* engine, std::string_view path);