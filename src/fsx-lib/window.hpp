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

    // setters
    void setTitle(const char* title);
    void setSize(unsigned int width, unsigned int height);
    void setResizable(bool resizable);
    void setFullscreen(bool fullscreen);
    void setBorderless(bool borderless);

    // getters
    std::string getTitle();
    int getWidth(), getHeight();
    bool getResizable(), getFullscreen(), getBorderless();
    int getID();

    static inline std::vector<Window*> openWindows;

    enum class Event {

        SHOWN                  = 0x202,
        HIDDEN                 = 0x203,
        EXPOSED                = 0x204,
        MOVED                  = 0x205,
        RESIZED                = 0x206,
        PIXEL_SIZE_CHANGED     = 0x207,
        METAL_VIEW_RESIZED     = 0x208,
        MINIMIZED              = 0x209,
        MAXIMIZED              = 0x20A,
        RESTORED               = 0x20B,
        MOUSE_ENTER            = 0x20C,
        MOUSE_LEAVE            = 0x20D,
        FOCUS_GAINED           = 0x20E,
        FOCUS_LOST             = 0x20F,
        CLOSE_REQUESTED        = 0x210,
        HIT_TEST               = 0x211,
        ICCPROF_CHANGED        = 0x212,
        DISPLAY_CHANGED        = 0x213,
        DISPLAY_SCALE_CHANGED  = 0x214,
        SAFE_AREA_CHANGED      = 0x215,
        OCCLUDED               = 0x216,
        ENTER_FULLSCREEN       = 0x217,
        LEAVE_FULLSCREEN       = 0x218,
        DESTROYED              = 0x219,
        HDR_STATE_CHANGED      = 0x21A,
        SETTINGS_CHANGED       = 0x21B,

        FIRST                  = SHOWN,            // 0x202
        LAST                   = SETTINGS_CHANGED, // 0x21B

    };

    bool pollEvent(Event windowEvent);

    // TODO: the window object will own the spawnchain
    // since swapchains are window specific.

private: 

    friend class Core;

    SDL_Window* windowObject_;
    uint32_t ID_;

    VkSurfaceKHR surface_;

};