#include <algorithm>
#include <SDL3/SDL.h>

#include "fsx.hpp"

void fsx::Window::open() {

    if (std::ranges::contains(openWindows, this) || windowObject_) {

        Utils::showWarning(
            ".open() was called on window \"{}\", but it was already open. "
            "ignoring the function call.", title
        );
        
        return;

    }

    windowObject_ = SDL_CreateWindow(title, width, height, SDL_WINDOW_VULKAN);

    if (!windowObject_)
        Utils::showWarning("failed to create SDL window object for window \"{}\".", title);
    else Utils::log("opened window \"{}\"", title);

    ID_ = static_cast<uint32_t>(SDL_GetWindowID(windowObject_));
    openWindows.push_back(this);

}

// polls basic window related events like resizing and stuff
// also updates the window properties owned by the fsx::Window object
void fsx::Window::pollEvents() {

    for (auto &event : fsx::events) {
        if (event.window.windowID == ID_) {

            if (event.type == SDL_EVENT_WINDOW_CLOSE_REQUESTED)
                close();

            if (event.type == SDL_EVENT_WINDOW_RESIZED) {

                int w, h;
                SDL_GetWindowSizeInPixels(windowObject_, &w, &h);
                width = static_cast<uint32_t>(w);
                height = static_cast<uint32_t>(h);

            }

        }
    }

    if (SDL_GetWindowFlags(windowObject_) & SDL_WINDOW_RESIZABLE) resizable = true;
    if (SDL_GetWindowFlags(windowObject_) & SDL_WINDOW_FULLSCREEN) fullscreen = true;
    if (SDL_GetWindowFlags(windowObject_) & SDL_WINDOW_BORDERLESS) borderless = true;

}

// fetches all window properties from the SDL window object and applies them
// to the fsx::Window object to make sure they are in sync
// must be called after the fsx::loop() to allow for the 
// properties to be modified on a per frame basis
void fsx::Window::applyProperties() {

    if (title != SDL_GetWindowTitle(windowObject_))
        SDL_SetWindowTitle(windowObject_, title);

    int w, h;
    SDL_GetWindowSizeInPixels(windowObject_, &w, &h);
    if (width != w || height != h)
        SDL_SetWindowSize(windowObject_, width, height);

    if (resizable != (SDL_GetWindowFlags(windowObject_) & SDL_WINDOW_RESIZABLE))
        SDL_SetWindowResizable(windowObject_, resizable);

    if (fullscreen != (SDL_GetWindowFlags(windowObject_) & SDL_WINDOW_FULLSCREEN))
        SDL_SetWindowFullscreen(windowObject_, fullscreen);

    if (borderless != (SDL_GetWindowFlags(windowObject_) & SDL_WINDOW_BORDERLESS))
        SDL_SetWindowBordered(windowObject_, !borderless);

}

void fsx::Window::close() {

    SDL_DestroyWindow(windowObject_);
    std::erase(openWindows, this);

    Utils::log("closed window \"{}\"", title);

}

// the deconstructor calls close() to ensure that the window pointer that 
// allows access to all instances of windows to the rest of the library
// does not outlast the actual window.
fsx::Window::~Window() {

    if (openWindows.size() > 0) {

        Utils::showWarning(
            "window \"{}\" was destroyed without .close() or user input. "
            "this can happen if the variable holding the window goes out of scope. "
            "make sure the window is not a local variable.", title
        );

        close();

    }

}