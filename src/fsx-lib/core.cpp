#include <vector>
#include <vulkan/vulkan.h>
#include <SDL3/SDL.h>
#include <SDL3/SDL_vulkan.h>

#define VOLK_IMPLEMENTATION
#include <Volk/volk.h>

#include "fsx.hpp"

void fsx::Core::initSDL() {

    if (!SDL_Init(SDL_INIT_VIDEO))
        throw std::runtime_error("failed to initialize SDL");
    else Utils::log("SDL initialized");

    Input::keyStates = SDL_GetKeyboardState(nullptr);

}

void fsx::Core::initVulkan() {

    Utils::log("beginning vulkan initialization");

    createInstance();
    createSurfaces(); // for each window

    Utils::log("finished vulkan initialization");

}

void fsx::Core::mainLoop() {

    while (!fsx::shouldAppClose) {

        if (Window::openWindows.empty()) {
            Utils::log("all windows closed, terminating application..");
            fsx::shouldAppClose = true;
        }

        // call SDL poll events and store them in a vector
        fsx::events.clear();
        SDL_Event e;
        while (SDL_PollEvent(&e)) 
            fsx::events.push_back(e);

        // get mouse button states and mouse position relative to window
        // keyboard states don't need to be called every frame so they're in Core::initSDL()
        Input::mouseStates = SDL_GetMouseState(&Input::mousePos.x, &Input::mousePos.y);

        for (auto *window : Window::openWindows)
            window->pollEvents(); // poll window events for every instance of window

        // fsx::loop()
        fsx::loop();

    }

}

void fsx::Core::cleanup() {

    

}

void fsx::Core::createInstance() {

    Core::appName_ = "fsx-lib application";
    Core::vulkanVersion_ = VK_API_VERSION_1_4;

    VkApplicationInfo appInfo {
        .sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
        .pApplicationName = appName_,
        .apiVersion = vulkanVersion_
    };

    uint32_t extensionCount = 0;
    char const* const* instanceExtensions = SDL_Vulkan_GetInstanceExtensions(&extensionCount);
    std::vector<const char*> requestedExtensions = { VK_EXT_DEBUG_UTILS_EXTENSION_NAME };

    for (int i = 0; i < extensionCount; i++)
        requestedExtensions.push_back(instanceExtensions[i]);

    Utils::log("requested extensions: {}", requestedExtensions.size());
    for (auto &extension : requestedExtensions)
        Utils::log(" |- {}", extension);

    std::vector<const char*> requestedLayers = { 
        "VK_LAYER_KHRONOS_validation" 
    };

    Utils::log("requested layers: {}", requestedLayers.size());
    for (auto &layer : requestedLayers)
        Utils::log(" |- {}", layer);

    VkDebugUtilsMessengerCreateInfoEXT debugInfo {
        .sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT,
        .messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
                           VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
                           VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT,
        .messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
                       VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT,
        .pfnUserCallback = debugCallback
    };

    VkInstanceCreateInfo instanceCI {
        .sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
        .pNext = &debugInfo,
        .pApplicationInfo = &appInfo,
        .enabledLayerCount = static_cast<uint32_t>(requestedLayers.size()),
        .ppEnabledLayerNames = requestedLayers.data(),
        .enabledExtensionCount = static_cast<uint32_t>(requestedExtensions.size()),
        .ppEnabledExtensionNames = requestedExtensions.data()
    };

    volkInitialize();

    if (Utils::checkResult(
        vkCreateInstance(&instanceCI, nullptr, &vulkanInstance_)) != VK_SUCCESS
    ) {
        throw std::runtime_error("failed to create vulkan instance");
    }
    else {

        volkLoadInstance(vulkanInstance_);
        Utils::log("created vulkan instance");

    }

}

// not sure if this should throw.
// because if there are multiple windows, and only 1 fails, i don't think that
// it should halt the entire program..
void fsx::Core::createSurfaces() {

    for (auto *window : Window::openWindows) {

        bool result = SDL_Vulkan_CreateSurface(window->windowObject_, vulkanInstance_, nullptr, &window->surface_);
        if (result == false)
            Utils::showError("failed to create vulkan surface for window \"{}\"", window->getTitle());
        else
            Utils::log("created vulkan surface for window \"{}\"", window->getTitle());

    }

}

VKAPI_ATTR VkBool32 VKAPI_CALL fsx::Core::debugCallback(
    VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
    VkDebugUtilsMessageTypeFlagsEXT messageType,
    const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData,
    void *pUserData
) {

    if (messageSeverity >= VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT)
        std::println(stderr, "validation layer: {}", pCallbackData->pMessage);

    return VK_FALSE;

}