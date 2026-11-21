#include <vector>
#include <vulkan/vulkan.h>
#include <SDL3/SDL.h>
#include <SDL3/SDL_vulkan.h>

#define VOLK_IMPLEMENTATION
#include <Volk/volk.h>

#include "fsx.hpp"

bool fsx::Core::running() {

    try {

        if ((!appRunning_) && Window::openWindows_.size() > 0) {

            appRunning_ = true;

            initSDL();
            initVulkan();

        }

        if (Window::openWindows_.empty()) {
            Utils::log("no windows open. terminating application..");
            appRunning_ = false;
        }

        // get mouse button states and mouse position relative to window
        // keyboard states don't need to be called every frame so they're in Core::initSDL()
        Input::mouseStates_ = SDL_GetMouseState(&Input::mousePos_.x, &Input::mousePos_.y);

        if (!appRunning_)
            cleanup();

    } catch (const std::exception &e) {

        fsx::Utils::showError("{}", e.what());

    }

    return appRunning_;
    
}

void fsx::Core::initSDL() {

    if (!SDL_Init(SDL_INIT_VIDEO))
        throw std::runtime_error("failed to initialize SDL");
    else Utils::log("SDL initialized");

    Input::keyStates_ = SDL_GetKeyboardState(nullptr);

}

void fsx::Core::initVulkan() {

    Utils::log("beginning vulkan initialization");

        createInstance();
        createSurfaces(); // for each window
        findPhysicalDevice();
        checkSurfaceFormats(); // for each window's surface
        findGraphicsQueueIndex();
        createLogicalDevice();

    Utils::log("finished vulkan initialization");

}

bool fsx::Core::pollEvents() {

    bool stillPolling = SDL_PollEvent(&event_);

    // this is just to make the X close button work
    // not sure if i should put it here or let the app explicitly define it instead
    for (auto &window : Window::openWindows_) {
        if (window->pollEvent(Window::Event::CLOSE_REQUESTED))
            window->close();
    }

    return stillPolling;

}

void fsx::Core::cleanup() {

    volkFinalize();
    SDL_Quit();

}

void fsx::Core::createInstance() {

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

    for (auto &window : Window::openWindows_) {

        bool result = SDL_Vulkan_CreateSurface(window->windowObject_, vulkanInstance_, nullptr, &window->surface_);
        if (result == false) {

            throw std::runtime_error(std::format(
                "failed to create vulkan surface for window \"{}\"", window->getTitle()
            ));

        } else
            Utils::log("created vulkan surface for window \"{}\"", window->getTitle());

    }

}

// finds the first discrete GPU that it finds
// if there isn't one, it instead selects the first GPU that it found
// throws if no GPU was found at all
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
    if (physicalDevice_ == nullptr && physicalDevices.size() > 0)
        physicalDevice_ = physicalDevices[0];

    if (physicalDevice_ == nullptr)
        throw std::runtime_error("failed to find suitable physical device");
    else {
        
        vkGetPhysicalDeviceProperties2(physicalDevice_, &deviceProperties);
        Utils::log("selected physical device: {}", deviceProperties.properties.deviceName);

    }

}

// makes sure that the selected GPU supports the format
void fsx::Core::checkSurfaceFormats() {

    for (auto &window : Window::openWindows_) {

        uint32_t formatCount = 0;
        vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice_, window->surface_, &formatCount, nullptr);

        std::vector<VkSurfaceFormatKHR> surfaceFormats(formatCount);
        vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice_, window->surface_, &formatCount, surfaceFormats.data());

        bool formatFound = false;
        for (const auto &surfaceFormat : surfaceFormats) {

            if (surfaceFormat.format == swapchainFormat_) {
                formatFound = true;
                break;
            }

        }

        if (formatFound)
            Utils::log("window \"{}\" surface supports swapchain format", window->getTitle());
        else {

            throw std::runtime_error(std::format(
                "window \"{}\" surface does not support swapchain format", window->getTitle()
            ));

        }

    }

}

void fsx::Core::findGraphicsQueueIndex() {

    uint32_t queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties2(physicalDevice_, &queueFamilyCount, nullptr);

    std::vector<VkQueueFamilyProperties2> queueFamilyProperties(queueFamilyCount,
        { .sType = VK_STRUCTURE_TYPE_QUEUE_FAMILY_PROPERTIES_2 }
    );

    vkGetPhysicalDeviceQueueFamilyProperties2(physicalDevice_, &queueFamilyCount, queueFamilyProperties.data());

    for (int currentFamilyIndex = 0; currentFamilyIndex < queueFamilyProperties.size(); currentFamilyIndex++) {
        for (const auto &window : Window::openWindows_) {

            VkBool32 hasPresentSupport = VK_FALSE;
            vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice_, currentFamilyIndex, window->surface_, &hasPresentSupport);

            const auto &properties = queueFamilyProperties[currentFamilyIndex];
            if (properties.queueFamilyProperties.queueFlags & VK_QUEUE_GRAPHICS_BIT && hasPresentSupport) {

                Core::graphicsQueueFamilyIndex_ = currentFamilyIndex;
                Utils::log("graphics queue family index: {}", Core::graphicsQueueFamilyIndex_);
                return;

            }

        }
    }

    throw std::runtime_error("could not find graphics queue index");

}

void fsx::Core::createLogicalDevice() {

    // query supported features
    // this linked list is made specifically to check if certain features are available.
    // this list won't be used for actually creating the logical device
    VkPhysicalDeviceVulkan14Features supportedFeatures14 {
        .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_4_FEATURES,
        .pNext = nullptr
    };

    VkPhysicalDeviceVulkan13Features supportedFeatures13 {
        .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_3_FEATURES,
        .pNext = &supportedFeatures14
    };

    VkPhysicalDeviceVulkan12Features supportedFeatures12 {
        .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_2_FEATURES,
        .pNext = &supportedFeatures13
    };

    VkPhysicalDeviceFeatures2 supportedFeatures {
        .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2,
        .pNext = &supportedFeatures12
    };

    vkGetPhysicalDeviceFeatures2(physicalDevice_, &supportedFeatures);

    if (
        !supportedFeatures13.dynamicRendering ||
        !supportedFeatures13.synchronization2 ||
        !supportedFeatures12.timelineSemaphore
    ) {

        throw std::runtime_error("physical device does not meet the feature requirements");

    } else
        Utils::log("physical device meets feature requirements");

    // this list is the one actually being used to create the logical device
    VkPhysicalDeviceVulkan14Features features14 {
        .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_4_FEATURES,
        .pNext = nullptr
    };

    VkPhysicalDeviceVulkan13Features features13 {
        .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_3_FEATURES,
        .pNext = &features14,
        .synchronization2 = VK_TRUE,
        .dynamicRendering = VK_TRUE
    };

    VkPhysicalDeviceVulkan12Features features12 {
        .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_2_FEATURES,
        .pNext = &features13,
        .timelineSemaphore = VK_TRUE
    };

    VkPhysicalDeviceFeatures2 features {
        .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2,
        .pNext = &features12
    };

    std::vector<float> queuePriorities = { 1.0f };
    VkDeviceQueueCreateInfo graphicsQueueCI {
        .sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
        .queueFamilyIndex = graphicsQueueFamilyIndex_,
        .queueCount = 1,
        .pQueuePriorities = queuePriorities.data()
    };

    const std::vector<const char*> deviceExtensions{ VK_KHR_SWAPCHAIN_EXTENSION_NAME };
    VkDeviceCreateInfo deviceCI {
        .sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
        .pNext = &features,
        .queueCreateInfoCount = 1,
        .pQueueCreateInfos = &graphicsQueueCI,
        .enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size()),
        .ppEnabledExtensionNames = deviceExtensions.data(),
        .pEnabledFeatures = nullptr // features struct is set in pNext (for some reason ????)
    };

    VkResult result = Utils::checkResult(
        vkCreateDevice(physicalDevice_, &deviceCI, nullptr, &logicalDevice_)
    );

    if (result != VK_SUCCESS)
        throw std::runtime_error("could not create logical device");
    else
        Utils::log("created logical device");

    vkGetDeviceQueue(logicalDevice_, graphicsQueueFamilyIndex_, 0, &graphicsQueue_);
    if (!graphicsQueue_)
        Utils::showError("could not get the graphics queue");

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