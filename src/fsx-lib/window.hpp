#pragma once

#include <vector>
#include <cstdint>
#include <SDL3/SDL.h>
#include <vulkan/vulkan.h>

class fsx::Window {

public:

    const char* title = "";
    uint32_t width = 1080;
    uint32_t height = 720;
    uint32_t targetFps = 144;
    
    bool resizable = true;
    bool fullscreen = false;
    bool borderless = false;

    static inline std::vector<Window*> openWindows;

    void open();
    void close();

    uint32_t getID() { return ID_; }

    // TODO: the window object will own the spawnchain
    // since swapchains are window specific.

private: 

    friend class Core;

    SDL_Window* windowObject_;
    uint32_t ID_;

    VkSurfaceKHR surface_;

    void pollEvents();
    void applyProperties();

    ~Window();

};