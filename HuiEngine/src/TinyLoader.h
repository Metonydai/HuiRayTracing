#pragma once

#include "StructType.h"

class ComputeShaderApplication;

class LoadedObj {
public:
    GPUMeshBuffers meshBuffers;
    uint32_t startIndex;
    uint32_t count;

    ComputeShaderApplication* creator;

    ~LoadedObj() { clearAll(); };
private:
    void clearAll();
};

std::shared_ptr<LoadedObj> loadModel(ComputeShaderApplication* engine, std::string_view path);