#pragma once

#include <vulkan/vulkan.h>

class fsx::Core {

public:

    // TODO: buddy these should NOT be public are you dumb
    static void initSDL();
    static void initVulkan();
    static void mainLoop();
    static void cleanup();

private:

    static inline const char *appName_;
    static inline uint32_t vulkanVersion_;

    static inline VkInstance vulkanInstance_ = nullptr;
    static inline VkPhysicalDevice physicalDevice_ = nullptr;

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