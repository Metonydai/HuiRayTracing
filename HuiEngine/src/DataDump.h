#pragma once

#include "Camera.h"

#include <vector>
#include <glm/glm.hpp>

#include <vulkan/vulkan.h>
#include <iostream>

struct PushConstantData
{
	uint32_t screenSize[2];
	uint32_t hittableCount;
	//uint32_t sampleStart;
	uint32_t samplesPerPixel;
	//uint32_t totalSamples;
	uint32_t maxDepth;
};

template<typename Ty, typename GlslTy, typename DumpTy>
class DataDump
{
protected:
	std::vector<Ty*> handles;
public:
	std::vector<GlslTy> heads;
	std::vector<DumpTy> dump;
	DataDump() = default;
	template<typename Derived, typename ...Args>
	Derived* Allocate(Args&&... args)
	{
		static_assert(std::is_base_of_v<Ty, Derived>, "need to derive from Ty");
		auto ret = new Derived(std::forward<Args>(args)...);
		ret->ptr = (uint32_t)handles.size();
		handles.push_back(ret);
		return ret;
	}
	void Clear()
	{
		for (auto handle : handles) delete handle;
		handles.clear();
	}
	~DataDump()
	{
		Clear();
	}
	uint32_t HeadSize()
	{
		return (uint32_t)heads.size() * sizeof(GlslTy);
	}
	uint32_t DumpSize()
	{
		return (uint32_t)dump.size() * sizeof(DumpTy);
	}

	void WriteMemory(VkDevice device, VkDeviceMemory headMemory, VkDeviceMemory dumpMemory)
	{
		WriteMemory(device, headMemory, heads.data(), HeadSize());
		WriteMemory(device, dumpMemory, dump.data(), DumpSize());
	}
private:
	void WriteMemory(VkDevice device, VkDeviceMemory memory, void* dataBlock, VkDeviceSize size)
	{
		void* data;
		if (vkMapMemory(device, memory, 0, size, 0, &data) != VK_SUCCESS)
			throw std::runtime_error("failed to map memory");
		memcpy(data, dataBlock, size);
		vkUnmapMemory(device, memory);
	}
};

/*
enum class MaterialType : uint32_t
{
	None = 0, Lambertian, Metal, Dielectrics
};

struct GLSLMaterial
{
	MaterialType type = MaterialType::None;
	uint32_t ptr;
};
*/

enum class HittableType : uint32_t
{
	None = 0, TriangleMesh, Sphere
};

struct GLSLHittable
{
	HittableType type = HittableType::None;
	uint32_t ptr;
	uint32_t mat = 0;
};

class Hittable
{
	friend class HittableDump;
	friend class DataDump<Hittable, GLSLHittable, glm::vec4>;
public:
	HittableType type;
	virtual std::vector<glm::vec4> Dump() = 0;
	//Material* mat;
private:
	uint32_t ptr;
};

class Sphere : public Hittable
{
public:
	Sphere(const glm::vec3& center, float radius)
		:center(center), radius(radius)
	{
		this->type = HittableType::Sphere;
	}
	glm::vec3 center;
	float radius;
	virtual std::vector<glm::vec4> Dump() override
	{
		return { glm::vec4(center, radius) };
	}
};

class HittableDump : public DataDump<Hittable, GLSLHittable, glm::vec4>
{
public:
	uint32_t Count() { return (uint32_t)handles.size(); }
	void Dump()
	{
		heads.clear();
		heads.reserve(handles.size());
		for (auto handle : handles)
		{
			heads.push_back({ handle->type, handle->ptr, /*handle->mat->ptr*/0 });
			auto vec4s = handle->Dump();
			for (const auto& v : vec4s) dump.push_back(v);
		}

	}
};

