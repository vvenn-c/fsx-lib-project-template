#pragma once

#include <vector>
#include <SDL3/SDL.h>

namespace fsx {

    class Utils;
    class Input;
    class Window;
    class Core;
    class Shaders;
    class Primitives;
    class Camera;
    class UI;

    void init();
    void loop();
    void exit();

    inline bool shouldAppClose = false;
    inline std::vector<SDL_Event> events; // i think i want to make sure this is always accecible from the app
    inline int run();

};

#include "utils.hpp"
#include "input.hpp"
#include "window.hpp"
#include "core.hpp"

int fsx::run() {

    try {

        fsx::init();

        fsx::Core::initSDL();
        fsx::Core::initVulkan();
        fsx::Core::mainLoop();
        fsx::Core::cleanup();

        fsx::exit();

    } catch (const std::exception &e) {

        fsx::Utils::showError("{}", e.what());
        return EXIT_FAILURE;

    } return EXIT_SUCCESS;

}