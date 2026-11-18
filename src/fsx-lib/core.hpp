#pragma once

#include <vulkan/vulkan.h>

class fsx::Core {

public:

    static int run();

private:

    friend class Window;
    friend class Input;

    static inline bool appStarted_ = false;
    static inline bool shouldAppClose_ = false;
    static inline std::vector<SDL_Event> events_;

    static inline const char *appName_;
    static inline uint32_t vulkanVersion_;

    static inline VkInstance vulkanInstance_ = nullptr;
    static inline VkPhysicalDevice physicalDevice_ = nullptr;

    // main
    static void initSDL();
    static void initVulkan();
    static void mainLoop();
    static void cleanup();

    // vulkan init helpers
    static void createInstance();
    static void createSurfaces();
    static void findPhysicalDevice();

    static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
        VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
        VkDebugUtilsMessageTypeFlagsEXT messageType,
        const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData,
        void *pUserData
    );

    Core() = delete;

};