#pragma once

#include "VulkanTools.h"
#include "VulkanInitializers.hpp"

#include "Metaballs.h"
#include "Camera.h"
#include "EditorCamera.h"
#include "StructType.h"
#include "TinyLoader.h"
#include "FrameRateCounter.h"
#include "VulkanTexture.h"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>
#include <fstream>
#include <memory>
#include <stdexcept>
#include <algorithm>
#include <chrono>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <cstdint>
#include <limits>
#include <array>
#include <optional>
#include <set>
#include <random>
#include <string_view>

static bool screenshotRequested = false;

const uint32_t WIDTH = 1600;
const uint32_t HEIGHT = 1200;

const uint32_t PARTICLE_COUNT = 1;

constexpr int MAX_FRAMES_IN_FLIGHT = 2;

struct SwapChainSupportDetails {
    VkSurfaceCapabilitiesKHR capabilities;
    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR> presentModes;
};

struct UniformBufferObject {
    glm::mat4 viewProjection;
    glm::vec4 spheres[20]; // {vec3(center), radius}
    glm::vec4 camPos;
    int sphereCount;
};

struct PushConstant {
    VkDeviceAddress vertexBufferAddress;
    VkDeviceAddress indexBufferAddress;
    float isoLevel = 1.0;
    float boxLength;
};

struct Material {
    // Parameter block used as push constant block
    struct PushBlock {
        float roughness = 0.0f;
        float metallic = 0.0f;
        float specular = 0.0f;
        float r, g, b;
    } params;
    std::string name;
    Material() {};
    Material(std::string n, glm::vec3 c) : name(n) {
        params.r = c.r;
        params.g = c.g;
        params.b = c.b;
    };
};

struct QueueFamilyIndices {
    std::optional<uint32_t> graphicsAndComputeFamily;
    std::optional<uint32_t> presentFamily;

    bool isComplete() {
        return graphicsAndComputeFamily.has_value() && presentFamily.has_value();
    }
};

class VulkanEngine {
public:
    void run();
    VkDevice GetDevice() { return device; }

    inline static GLFWwindow* GetWindow() { return window; }

    GPUMeshBuffers UploadMesh(const std::vector<uint32_t>& indices, const std::vector<Vertex>& vertices);

    std::shared_ptr<LoadedObj> cube;

    VkPhysicalDeviceFeatures enabledFeatures;
    VkPhysicalDeviceProperties properties;
public:
    struct StorageImage {
        uint32_t width;
        uint32_t height;
        VkFormat format;
        VkImage image;
        VkDeviceMemory deviceMemory;
        VkImageView view;
        VkSampler sampler;
        VkDescriptorSet descriptorSet;
    };

    StorageImage storageImages[MAX_FRAMES_IN_FLIGHT];
public:
    struct StorageVoxels {
        uint32_t width;
        uint32_t height;
        uint32_t depth;
        VkFormat format;
        VkImage image;
        VkDeviceMemory deviceMemory;
        VkImageView view;
        VkDescriptorSet descriptorSet;
    };

    FrameRateCounter counter{};

    uint32_t maxVertices = 500000, maxIndices = maxVertices * 3;
    VkBuffer vertexBuffer[MAX_FRAMES_IN_FLIGHT], indexBuffer[MAX_FRAMES_IN_FLIGHT], counterBuffer[MAX_FRAMES_IN_FLIGHT];
    VkDeviceMemory vertexMemory[MAX_FRAMES_IN_FLIGHT], indexMemory[MAX_FRAMES_IN_FLIGHT], counterMemory[MAX_FRAMES_IN_FLIGHT];

    StorageVoxels storageVoxels[MAX_FRAMES_IN_FLIGHT];

    UniformBufferObject ubo{};
    PushConstant pushConstant{};

    const uint32_t imageSize = 400;
    Metaballs metaballs;

    VkPipeline boundingBoxPipeline;
    VkPipeline wireframePipeline;
    VkPipelineLayout wireframePipelineLayout;
    AllocatedBuffer wireVBuffer;
    AllocatedBuffer wireIBuffer;

public:
    void createBoundingBoxPipeline();
    void createWireframePipeline();
    void createScene();

public:

    bool displaySkybox = true;
    struct Textures {
        huiluna::TextureCubeMap environmentCube;
        // Generated at runtime
        huiluna::Texture2D lutBrdf;
        huiluna::TextureCubeMap irradianceCube;
        huiluna::TextureCubeMap prefilteredCube;
    } textures;

    struct {
        VkPipeline skybox{ VK_NULL_HANDLE };
        VkPipeline pbr{ VK_NULL_HANDLE };
    } pipelines;

    struct {
        VkDescriptorSet object{ VK_NULL_HANDLE };
        VkDescriptorSet skybox{ VK_NULL_HANDLE };
    } pbrDescriptorSets;

    struct {
        AllocatedBuffer object;
        AllocatedBuffer skybox;
        AllocatedBuffer params;
    } pbrUniformBuffers;

    struct UBOParams {
        glm::vec4 lights[4] = {};
        float exposure = 4.5f;
        float gamma = 2.2f;
    } uboParams;

    struct UBOMatrices {
        glm::mat4 projection;
        glm::mat4 view;
        glm::mat4 model;
        glm::vec3 camPos;
    } uboMatrices;

    VkPipelineLayout pbrPipelineLayout{ VK_NULL_HANDLE };
    VkDescriptorSetLayout pbrDescriptorSetLayout{ VK_NULL_HANDLE };

    // Default materials to select from
    std::vector<Material> materials;
    Material::PushBlock selectedMat{};
    int32_t materialIndex = 0;

    std::vector<std::string> materialNames;

    void createPbrIblPipeline();

    void preparePipelines();
    void prepareUniformBuffers();
    void setupDescriptors();
    void generateBRDFLUT();
    void generatePrefilteredCube();
    void generateIrradianceCube();

    struct DepthImage {
        VkImage image;
        VkDeviceMemory memory;
        VkImageView view;
    } depthImage;

public:

    Huiluna::EditorCamera m_Camera;

private:
    static GLFWwindow* window;

    VkInstance instance;
    VkDebugUtilsMessengerEXT debugMessenger;
    VkSurfaceKHR surface;

    VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
    VkDevice device;

    VkQueue graphicsQueue;
    VkQueue computeQueue;
    VkQueue presentQueue;

    VkSwapchainKHR swapChain;
    std::vector<VkImage> swapChainImages;
    VkFormat swapChainImageFormat;
    VkExtent2D swapChainExtent;
    std::vector<VkImageView> swapChainImageViews;
    std::vector<VkFramebuffer> swapChainFramebuffers;

    VkRenderPass renderPass;
    VkDescriptorSetLayout descriptorSetLayout;
    VkPipelineLayout pipelineLayout;
    VkPipeline graphicsPipeline;

    VkDescriptorSetLayout computeDescriptorSetLayout;
    VkPipelineLayout computePipelineLayout;
    VkPipeline computePipelineField;
    VkPipeline computePipelineMarching;
    VkPipelineCache pipelineCache;

    VkCommandPool commandPool;

    // For hittables and materials
    std::array<VkBuffer, 4> shaderStorageBuffers;
    std::array<VkDeviceMemory, 4> shaderStorageBuffersMemory;

    std::vector<VkBuffer> uniformBuffers;
    std::vector<VkDeviceMemory> uniformBuffersMemory;
    std::vector<void*> uniformBuffersMapped;

    VkDescriptorPool imguiPool;

    VkDescriptorPool descriptorPool;
    std::vector<VkDescriptorSet> descriptorSets;
    std::vector<VkDescriptorSet> computeDescriptorSets;

    std::vector<VkCommandBuffer> commandBuffers;
    std::vector<VkCommandBuffer> computeCommandBuffers;

    std::vector<VkSemaphore> imageAvailableSemaphores;
    std::vector<VkSemaphore> renderFinishedSemaphores;
    std::vector<VkSemaphore> computeFinishedSemaphores;
    std::vector<VkFence> inFlightFences;
    std::vector<VkFence> computeInFlightFences;
    uint32_t currentFrame = 0;

    double lastFrameTime = 0.0f;

    bool framebufferResized = false;

    double lastTime = 0.0f;
private:
    
    void initWindow();
    void initVulkan();
    void initImgui();
    void mainLoop();
    void cleanup();

private:
    void createInstance();
    void createSurface();
    void createLogicalDevice();
    void createSwapChain();
    void createDepthResources();

    void createPipelineCache();

    void createGraphicsPipeline();
    void createFramebuffers();
    void createImageViews();
    void createRenderPass();

    void createComputePipelines();

    void createCommandPool();
    void createCommandBuffers();
    void createComputeCommandBuffers();

    void createDescriptorPool();
    void createDescriptorSetLayout();
    void createComputeDescriptorSetLayout();
    void createDescriptorSets();
    void createComputeDescriptorSets();

    void createShaderStorageImages();
    void createShaderStorageVoxels();
    void createShaderStorageBuffers();
    void createUniformBuffers();

    void createSyncObjects();
public:    
    static std::vector<char> readFile(const std::string& filename);
    VkShaderModule createShaderModule(const std::vector<char>& code);

    void createImage(uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory);
    void createImageCubemap(uint32_t width, uint32_t height, uint32_t mipLevels, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory);
    void createImage3D(uint32_t width, uint32_t height, uint32_t depth, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory);

    void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
    void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory, bool addressBit = false);
    
    void insertImageMemoryBarrier(
        VkCommandBuffer cmdbuffer,
        VkImage image,
        VkAccessFlags srcAccessMask,
        VkAccessFlags dstAccessMask,
        VkImageLayout oldImageLayout,
        VkImageLayout newImageLayout,
        VkPipelineStageFlags srcStageMask,
        VkPipelineStageFlags dstStageMask,
        VkImageSubresourceRange subresourceRange = { VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1 });

    VkCommandBuffer beginSingleTimeCommands();
    void endSingleTimeCommands(VkCommandBuffer commandBuffer, VkQueue queue);
private:
    void drawFrame();
    void recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex);
    void recordComputeCommandBuffer(VkCommandBuffer commandBuffer);


    void updateUniformBuffers(uint32_t currentImage);
    void updatePushConstant(uint32_t currentImage);

private:
    void saveStorageImage(const StorageImage& swapChainImage, const std::string& outputDir = "result_images/", const std::string& filename = "Huiyu");

    void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
    void setupDebugMessenger();

    void cleanupSwapChain();
    void recreateSwapChain();

    void pickPhysicalDevice();
    uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);

    VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
    VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
    VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);

    SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device);
    bool isDeviceSuitable(VkPhysicalDevice device);
    bool checkDeviceExtensionSupport(VkPhysicalDevice device);
    QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);
    std::vector<const char*> getRequiredExtensions();
    bool checkValidationLayerSupport();

    static void framebufferResizeCallback(GLFWwindow* window, int width, int height);
    static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData);

private:
    void checkMeshShaderSupport(VkPhysicalDevice physicalDevice);
    inline bool hasStencilComponent(VkFormat format) {
        return format == VK_FORMAT_D32_SFLOAT_S8_UINT || format == VK_FORMAT_D24_UNORM_S8_UINT; }
    VkFormat findDepthFormat();
    VkFormat findSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features);

    PFN_vkCmdDrawMeshTasksEXT vkCmdDrawMeshTasksEXT{ VK_NULL_HANDLE };

    bool meshShaderSupport = false;
    bool taskShaderSupport = false;
};
