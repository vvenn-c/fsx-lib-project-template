#pragma once

#include <vector>
#include <cstdint>
#include <SDL3/SDL.h>
#include <vulkan/vulkan.h>

class fsx::Window {

public:

    Window();
    ~Window();

    void destroy();

    void setTitle(const char* title);
    void setSize(unsigned int width, unsigned int height);
    void setResizable(bool resizable);
    void setFullscreen(bool fullscreen);
    void setBorderless(bool borderless);

    std::string getTitle();
    int getWidth(), getHeight();
    bool getResizable(), getFullscreen(), getBorderless();
    int getID() { return ID_; }

    static inline std::vector<Window*> openWindows;

    enum class Event {
        
    };

    // TODO: the window object will own the spawnchain
    // since swapchains are window specific.

private: 

    friend class Core;

    SDL_Window* windowObject_;
    uint32_t ID_;

    VkSurfaceKHR surface_;

    void pollEvents();

};