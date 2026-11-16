#pragma once

#include <print>
#include <vulkan/vulkan.h>

class fsx::Core {

public:

    Core() = delete;

    static void initSDL();
    static void initVulkan();
    static void mainLoop();
    static void cleanup();

private:

    VkInstance vulkanInstance_;

    void render();

    // vulkan init helpers
    bool createInstance();

};