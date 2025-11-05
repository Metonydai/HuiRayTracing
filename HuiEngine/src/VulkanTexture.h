#pragma once

#include <vulkan/vulkan.h>

#include <string_view>

class VulkanEngine;

namespace huiluna
{
	class Texture
	{
	public:
		VulkanEngine*		  engine;
		VkImage               image;
		VkImageLayout         imageLayout;
		VkDeviceMemory        deviceMemory;
		VkImageView           view;
		uint32_t              width, height;
		uint32_t              mipLevels;
		uint32_t              layerCount;
		VkDescriptorImageInfo descriptor;
		VkSampler             sampler;

		void      updateDescriptor();
		void      destroy();
	};

	class Texture2D : public Texture
	{
	public:
		void loadFromFile(
			std::string_view   filename,
			VkFormat           format,
			VulkanEngine*      engine,
			VkQueue            copyQueue,
			VkImageUsageFlags  imageUsageFlags = VK_IMAGE_USAGE_SAMPLED_BIT,
			VkImageLayout      imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
	};

	class TextureCubeMap : public Texture
	{
	public:
		void loadFromFile(
			std::string_view   filename,
			VkFormat           format,
			VulkanEngine*      engine,
			VkQueue            copyQueue,
			VkImageUsageFlags  imageUsageFlags = VK_IMAGE_USAGE_SAMPLED_BIT,
			VkImageLayout      imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
	};
}