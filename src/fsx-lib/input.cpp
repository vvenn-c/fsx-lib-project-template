#include "fsx.hpp"

bool fsx::Input::keyPressed(Key key) {

    int scancode = static_cast<int>(key);
    for (auto &event : fsx::events) {

        if (event.type == SDL_EVENT_KEY_DOWN) {
            if (event.key.scancode == scancode)
                return true;
        }

    }

    return false;

}

bool fsx::Input::getKeyState(Key key) {
    return keyStates[static_cast<int>(key)];
}

bool fsx::Input::keyReleased(Key key) {

    int scancode = static_cast<int>(key);
    for (auto &event : fsx::events) {

        if (event.type == SDL_EVENT_KEY_UP) {
            if (event.key.scancode == scancode)
                return true;
        }

    }

    return false;

}

bool fsx::Input::mouseButtonPressed(Mouse button) {

    int scancode = static_cast<int>(button);
    for (auto &event : fsx::events) {

        if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
            if (event.button.button == scancode)
                return true;
        }

    }

    return false;

}

bool fsx::Input::getMouseState(Mouse button) {

    int scancode = static_cast<int>(button);
    return (mouseStates & SDL_BUTTON_MASK(scancode)) != 0; 

}

bool fsx::Input::mouseButtonReleased(Mouse button) {

    int scancode = static_cast<int>(button);
    for (auto &event : fsx::events) {

        if (event.type == SDL_EVENT_MOUSE_BUTTON_UP) {
            if (event.button.button == scancode)
                return true;
        }

    }

    return false;

}