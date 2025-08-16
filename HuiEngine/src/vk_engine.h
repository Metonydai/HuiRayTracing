#include "VulkanTools.h"
#include "VulkanInitializers.hpp"

#include "DataDump.h"
#include "Camera.h"
#include "EditorCamera.h"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>
#include <fstream>
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
    glm::mat4 viewInverse;
    glm::mat4 projInverse;
};

struct Particle {
    glm::vec2 position;
    glm::vec2 velocity;
    glm::vec4 color;

    static VkVertexInputBindingDescription getBindingDescription() {
        VkVertexInputBindingDescription bindingDescription{};
        bindingDescription.binding = 0;
        bindingDescription.stride = sizeof(Particle);
        bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

        return bindingDescription;
    }

    static std::array<VkVertexInputAttributeDescription, 3> getAttributeDescriptions() {
        std::array<VkVertexInputAttributeDescription, 3> attributeDescriptions{};

        attributeDescriptions[0].binding = 0;
        attributeDescriptions[0].location = 0;
        attributeDescriptions[0].format = VK_FORMAT_R32G32_SFLOAT;
        attributeDescriptions[0].offset = offsetof(Particle, position);

        attributeDescriptions[1].binding = 0;
        attributeDescriptions[1].location = 1;
        attributeDescriptions[1].format = VK_FORMAT_R32G32_SFLOAT;
        attributeDescriptions[1].offset = offsetof(Particle, velocity);

        attributeDescriptions[2].binding = 0;
        attributeDescriptions[2].location = 2;
        attributeDescriptions[2].format = VK_FORMAT_R32G32B32A32_SFLOAT;
        attributeDescriptions[2].offset = offsetof(Particle, color);

        return attributeDescriptions;
    }
};

struct QueueFamilyIndices {
    std::optional<uint32_t> graphicsAndComputeFamily;
    std::optional<uint32_t> presentFamily;

    bool isComplete() {
        return graphicsAndComputeFamily.has_value() && presentFamily.has_value();
    }
};

class ComputeShaderApplication {
public:
    void run();
public:
    HittableDump hittables;
    PushConstantData pushConstantData;

    void createScene();
    void writeMemoryFromHost();
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
    UniformBufferObject ubo{};

    Huiluna::EditorCamera m_Camera;

public:
    inline static GLFWwindow* GetWindow() { return window; }
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
    VkPipeline computePipeline;

    VkCommandPool commandPool;

    std::array<VkBuffer, 2> shaderStorageBuffers;
    std::array<VkDeviceMemory, 2> shaderStorageBuffersMemory;

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

    void createGraphicsPipeline();
    void createFramebuffers();
    void createImageViews();
    void createRenderPass();

    void createComputePipeline();

    void createCommandPool();
    void createCommandBuffers();
    void createComputeCommandBuffers();

    void createDescriptorPool();
    void createDescriptorSetLayout();
    void createComputeDescriptorSetLayout();
    void createDescriptorSets();
    void createComputeDescriptorSets();

    void createShaderStorageImages();
    void createShaderStorageBuffers();
    void createUniformBuffers();

    void createSyncObjects();
private:
    void drawFrame();
    void recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex);
    void recordComputeCommandBuffer(VkCommandBuffer commandBuffer);

    VkCommandBuffer beginSingleTimeCommands();
    void endSingleTimeCommands(VkCommandBuffer commandBuffer);

    void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
    void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);
    void updateUniformBuffers(uint32_t currentImage);
    
    void createImage(uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory);
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
    
    VkShaderModule createShaderModule(const std::vector<char>& code);
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

    static std::vector<char> readFile(const std::string& filename);
    static void framebufferResizeCallback(GLFWwindow* window, int width, int height);
    static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData);
};
