#include "fsx.hpp"

void fsx::Core::initSDL() {

    if (!SDL_Init(SDL_INIT_VIDEO))
        throw std::runtime_error("failed to initialize SDL");
    else Utils::log("SDL initialized");

    Input::keyStates = SDL_GetKeyboardState(NULL);

}

void fsx::Core::initVulkan() {



}

void fsx::Core::mainLoop() {

    // structure of the main loop:
    // 1. call SDL poll events
    //    poll window events for every instance of window
    // 2. fetch the SDL window properties and update the user-accessible variables with them
    // 3. fsx::loop() <- this will round up all the data that needs to be sent to vulkan for rendering
    // 4. use the user-accessible variables to update the SDL window properties
    // 5. reconstruct the swapchain if necessary
    // 6. other shader stuff idk yet (?)
    // 7. get the data prepared by fsx::loop() and use it to render with fsx::Core::render()

    while (!fsx::shouldAppClose) {

        if (Window::openWindows.empty()) {
            Utils::log("all windows closed, terminating application..");
            fsx::shouldAppClose = true;
        }

        Input::updateLastKeyStates();

        // 1. call SDL poll events
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            for (auto *window : Window::openWindows)
                window->pollEvents(event); // poll window events for every instance of window
        }

        // 3. fsx::loop() <- this will round up all the data that needs to be sent to vulkan for rendering
        fsx::loop();

        // 4. use the user-accessible variables to update the SDL window properties
        for (auto *window : Window::openWindows)
            window->applyProperties();

    }

}

void fsx::Core::cleanup() {

    

}