#pragma once

#include <vector>
#include <cstdint>
#include <SDL3/SDL.h>

class fsx::Window {

public:

    ~Window();

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

protected: 

    friend class Core;

    SDL_Window* windowObject_;
    uint32_t ID_;

    void pollEvents();
    void applyProperties();

};