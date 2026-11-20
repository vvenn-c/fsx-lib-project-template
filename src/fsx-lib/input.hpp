#pragma once

#include <SDL3/SDL.h>
#include <glm/glm.hpp>

#include "fsx.hpp"

class fsx::Input {

public:

    enum class Key : uint8_t {

        UNKNOWN = 0,

        A = 4,
        B = 5,
        C = 6,
        D = 7,
        E = 8,
        F = 9,
        G = 10,
        H = 11,
        I = 12,
        J = 13,
        K = 14,
        L = 15,
        M = 16,
        N = 17,
        O = 18,
        P = 19,
        Q = 20,
        R = 21,
        S = 22,
        T = 23,
        U = 24,
        V = 25,
        W = 26,
        X = 27,
        Y = 28,
        Z = 29,

        NUM_1 = 30,
        NUM_2 = 31,
        NUM_3 = 32,
        NUM_4 = 33,
        NUM_5 = 34,
        NUM_6 = 35,
        NUM_7 = 36,
        NUM_8 = 37,
        NUM_9 = 38,
        NUM_0 = 39,

        RETURN = 40,
        ESCAPE = 41,
        BACKSPACE = 42,
        TAB = 43,
        SPACE = 44,

        MINUS = 45,
        EQUALS = 46,
        LEFTBRACKET = 47,
        RIGHTBRACKET = 48,
        BACKSLASH = 49,
        SEMICOLON = 51,
        APOSTROPHE = 52,
        GRAVE = 53,
        COMMA = 54,
        PERIOD = 55,
        SLASH = 56,

        CAPSLOCK = 57,

        F1 = 58,
        F2 = 59,
        F3 = 60,
        F4 = 61,
        F5 = 62,
        F6 = 63,
        F7 = 64,
        F8 = 65,
        F9 = 66,
        F10 = 67,
        F11 = 68,
        F12 = 69,

        PRINTSCREEN = 70,

        HOME = 74,
        PAGEUP = 75,
        DELETE = 76,
        END = 77,
        PAGEDOWN = 78,
        RIGHT = 79,
        LEFT = 80,
        DOWN = 81,
        UP = 82,

        MUTE = 127,
        VOLUMEUP = 128,
        VOLUMEDOWN = 129,

        LCTRL = 224,
        LSHIFT = 225,
        LALT = 226,
        LGUI = 227,
        RCTRL = 228,
        RSHIFT = 229,
        RALT = 230,
        RGUI = 231,

        NUMLOCK_CLEAR = 83,

        NUMPAD_DIVIDE = 84,
        NUMPAD_MULTIPLY = 85,
        NUMPAD_MINUS = 86,
        NUMPAD_PLUS = 87,
        NUMPAD_ENTER = 88,
        NUMPAD_1 = 89,
        NUMPAD_2 = 90,
        NUMPAD_3 = 91,
        NUMPAD_4 = 92,
        NUMPAD_5 = 93,
        NUMPAD_6 = 94,
        NUMPAD_7 = 95,
        NUMPAD_8 = 96,
        NUMPAD_9 = 97,
        NUMPAD_0 = 98,
        NUMPAD_PERIOD = 99

    };

    static bool keyPressed(Key key);
    static bool getKeyState(Key key);
    static bool keyReleased(Key key);

    enum class Mouse : uint8_t {

        LEFT   = 1,
        MIDDLE = 2,
        RIGHT  = 3,
        X1     = 4,
        X2     = 5

    };

    static bool mouseButtonPressed(Mouse button);
    static bool getMouseState(Mouse button);
    static bool mouseButtonReleased(Mouse button);

    static glm::vec2 getMousePos();

    // TODO: handle mouse scrolling
    // needa figure out how that works tho

private:

    friend class Core;

    static inline const bool *keyStates_;
    static inline SDL_MouseButtonFlags mouseStates_;
    static inline glm::vec2 mousePos_;

    Input() = delete;

};