#include <vector>
#include <vulkan/vulkan.h>
#include <SDL3/SDL.h>
#include <SDL3/SDL_vulkan.h>

#define VOLK_IMPLEMENTATION
#include <Volk/volk.h>

#include "fsx.hpp"

int fsx::Core::run() {

    try {

        fsx::init();

        if (!appStarted_) {

            appStarted_ = true;

            initSDL();
            initVulkan();
            mainLoop();
            cleanup();

        } else {

            Utils::showError(
                "fsx::Core::run() was called multiple times. "
                "this function should only be called once "
                "inside the main function."
            );

            // can't use throw to reliably terminate in case of a nested call.
            // so having to resort to violence here..
            std::exit(EXIT_FAILURE);

        }

        fsx::exit();

    } catch (const std::exception &e) {

        fsx::Utils::showError("{}", e.what());
        return EXIT_FAILURE;

    } return EXIT_SUCCESS;

}

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
    findPhysicalDevice();

    Utils::log("finished vulkan initialization");

}

void fsx::Core::mainLoop() {

    while (!shouldAppClose_) {

        if (Window::openWindows.empty()) {
            Utils::log("all windows closed, terminating application..");
            shouldAppClose_ = true;
        }

        // call SDL poll events and store them in a vector
        events_.clear();
        SDL_Event e;
        while (SDL_PollEvent(&e)) 
            events_.push_back(e);

        for (auto &window : Window::openWindows) {
            if (window->pollEvent(Window::Event::CLOSE_REQUESTED))
                window->destroy();
        }

        // get mouse button states and mouse position relative to window
        // keyboard states don't need to be called every frame so they're in Core::initSDL()
        Input::mouseStates = SDL_GetMouseState(&Input::mousePos.x, &Input::mousePos.y);

        // fsx::loop()
        fsx::loop();

    }

}

void fsx::Core::cleanup() {

    volkFinalize();
    SDL_Quit();

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

void fsx::Core::findPhysicalDevice() {

    uint32_t physicalDeviceCount = 0;
    vkEnumeratePhysicalDevices(vulkanInstance_, &physicalDeviceCount, nullptr);

    std::vector<VkPhysicalDevice> physicalDevices(physicalDeviceCount);
    vkEnumeratePhysicalDevices(vulkanInstance_, &physicalDeviceCount, physicalDevices.data());

    VkPhysicalDeviceProperties2 deviceProperties{
        .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PROPERTIES_2
    };

    Utils::log("found physical devices:");
    for (auto &device : physicalDevices) {

        vkGetPhysicalDeviceProperties2(device, &deviceProperties);
        Utils::log(" |- {}", deviceProperties.properties.deviceName);

    }

    for (auto &device : physicalDevices) {

        vkGetPhysicalDeviceProperties2(device, &deviceProperties);
        if (deviceProperties.properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) {
            physicalDevice_ = device;
            break;
        }

    }

    // if no discrete device found, pick the first one we got
    if (physicalDevices.size() > 0)
        physicalDevice_ = physicalDevices[0];

    if (physicalDevice_ == nullptr)
        throw std::runtime_error("failed to find suitable physical device");
    else {
        
        vkGetPhysicalDeviceProperties2(physicalDevice_, &deviceProperties);
        Utils::log("selected physical device: {}", deviceProperties.properties.deviceName);

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