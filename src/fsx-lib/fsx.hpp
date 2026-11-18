#pragma once

#include <vector>
#include <SDL3/SDL.h>

namespace fsx {

    class Utils;
    class Input;
    class Window;
    class Core;
    class Shader;
    class Primitive;
    class Camera;
    class UI;

    void init();
    void loop();
    void exit();

};

#include "utils.hpp"
#include "input.hpp"
#include "window.hpp"
#include "core.hpp"