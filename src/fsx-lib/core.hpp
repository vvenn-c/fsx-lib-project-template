#pragma once

#include <vulkan/vulkan.h>

class fsx::Core {

public:

    static int run(void (*init)() = nullptr, void (*loop)() = nullptr, void (*exit)() = nullptr);

private:

    friend class Window;
    friend class Input;

    static inline bool appStarted_ = false;
    static inline bool shouldAppClose_ = false;
    static inline std::vector<SDL_Event> events_;

    constexpr static inline const char *appName_ = "fsx-lib application";
    const static inline uint32_t vulkanVersion_ = VK_API_VERSION_1_4;

    static inline VkInstance vulkanInstance_ = nullptr;
    static inline VkPhysicalDevice physicalDevice_ = nullptr;
    static inline VkDevice logicalDevice_ = nullptr;

    constexpr static VkFormat swapchainFormat_ = VK_FORMAT_B8G8R8A8_SRGB;
    constexpr static VkFormat depthImageFormat_ = VK_FORMAT_D32_SFLOAT;

    static inline uint32_t graphicsQueueFamilyIndex_ = UINT32_MAX;
    static inline VkQueue graphicsQueue_ = nullptr;

    // main
    static void initSDL();
    static void initVulkan();
    static void mainLoop(void (*loop)() = nullptr);
    static void cleanup();

    // vulkan init helpers
    static void createInstance();
    static void createSurfaces();
    static void findPhysicalDevice();
    static void checkSurfaceFormats();
    static void findGraphicsQueueIndex();
    static void createLogicalDevice();

    static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
        VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
        VkDebugUtilsMessageTypeFlagsEXT messageType,
        const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData,
        void *pUserData
    );

    Core() = delete;

};