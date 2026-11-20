#include <algorithm>
#include <SDL3/SDL.h>

#include "fsx.hpp"

fsx::Window::Window() {

    windowObject_ = SDL_CreateWindow(
        "untitled", 1080, 720, 
        SDL_WINDOW_VULKAN | SDL_WINDOW_RESIZABLE
    );

    if (!windowObject_)
        Utils::showWarning("SDL failed to create a window object");

    ID_ = static_cast<uint32_t>(SDL_GetWindowID(windowObject_));
    openWindows_.push_back(this);

}

bool fsx::Window::pollEvent(Event windowEvent) {

    for (auto &event : Core::events_) {
        if (event.window.windowID == ID_) {

            if (event.type == static_cast<int>(windowEvent))
                return true;

        }
    }

    return false;

}

void fsx::Window::destroy() {

    Utils::log("closed window \"{}\"", getTitle());

    SDL_DestroyWindow(windowObject_);
    std::erase(openWindows_, this);

}

// setters
void fsx::Window::setTitle(const char* title) { SDL_SetWindowTitle(windowObject_, title); }
void fsx::Window::setSize(unsigned int width, unsigned int height) { SDL_SetWindowSize(windowObject_, width, height); }
void fsx::Window::setResizable(bool resizable) { SDL_SetWindowResizable(windowObject_, resizable); }
void fsx::Window::setFullscreen(bool fullscreen) { SDL_SetWindowFullscreen(windowObject_, fullscreen); }
void fsx::Window::setBorderless(bool borderless) { SDL_SetWindowBordered(windowObject_, !borderless); }

// getters
std::string fsx::Window::getTitle() { return SDL_GetWindowTitle(windowObject_); }
int fsx::Window::getWidth() { int w; SDL_GetWindowSizeInPixels(windowObject_, &w, nullptr); return w; }
int fsx::Window::getHeight() { int h; SDL_GetWindowSizeInPixels(windowObject_, nullptr, &h); return h; }
bool fsx::Window::getResizable() { return SDL_GetWindowFlags(windowObject_) & SDL_WINDOW_RESIZABLE; }
bool fsx::Window::getFullscreen() { return SDL_GetWindowFlags(windowObject_) & SDL_WINDOW_FULLSCREEN; }
bool fsx::Window::getBorderless() { return SDL_GetWindowFlags(windowObject_) & SDL_WINDOW_BORDERLESS; }

int fsx::Window::getID() { return ID_; }

// the deconstructor calls destroy() to ensure that the window pointer that 
// allows access to all instances of windows to the rest of the library
// does not outlast the actual window.
fsx::Window::~Window() {

    if (openWindows_.size() > 0) {

        Utils::showError(
            "window \"{}\" was destroyed without .destroy() or user input. "
            "this can happen if the variable holding the window goes out of scope. "
            "make sure the window is not a local variable.", getTitle()
        );

        destroy();
        std::exit(EXIT_FAILURE);

    }

}