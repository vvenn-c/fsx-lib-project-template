#pragma once

#include <vulkan/vulkan.h>

class fsx::Core {

public:

    Core() = delete;

    static void initSDL();
    static void initVulkan();
    static void mainLoop();
    static void cleanup();

private:

    const const char *appName_ = "fsx-lib application";
    const uint32_t vulkanVersion_ = VK_API_VERSION_1_4;

    VkInstance vulkanInstance_;

    void render();

    // vulkan init helpers
    bool createInstance();

    static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
        VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
        VkDebugUtilsMessageTypeFlagsEXT messageType,
        const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData,
        void *pUserData
    );

};