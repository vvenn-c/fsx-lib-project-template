#pragma once

#include <vulkan/vulkan.h>

class fsx::Core {

public:

    static void initSDL();
    static void initVulkan();
    static void mainLoop();
    static void cleanup();

private:

    static inline const char *appName_;
    static inline uint32_t vulkanVersion_;

    static inline VkInstance vulkanInstance_;

    // vulkan init helpers
    static void createInstance();
    static void createSurfaces();

    static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
        VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
        VkDebugUtilsMessageTypeFlagsEXT messageType,
        const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData,
        void *pUserData
    );

    Core() = delete;

};