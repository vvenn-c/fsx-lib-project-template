#include "fsx.hpp"

bool fsx::Input::keyPressed(Key key) {

    int scancode = static_cast<int>(key);

    if (Core::event_.type == SDL_EVENT_KEY_DOWN) {
        if (Core::event_.key.scancode == scancode)
            return true;
    }

    return false;

}

bool fsx::Input::getKeyState(Key key) {
    return keyStates_[static_cast<int>(key)];
}

bool fsx::Input::keyReleased(Key key) {

    int scancode = static_cast<int>(key);

    if (Core::event_.type == SDL_EVENT_KEY_UP) {
        if (Core::event_.key.scancode == scancode)
            return true;
    }

    return false;

}

bool fsx::Input::mouseButtonPressed(Mouse button) {

    int scancode = static_cast<int>(button);

    if (Core::event_.type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
        if (Core::event_.button.button == scancode)
            return true;
    }

    return false;

}

bool fsx::Input::getMouseState(Mouse button) {

    int scancode = static_cast<int>(button);
    return (mouseStates_ & SDL_BUTTON_MASK(scancode)) != 0; 

}

bool fsx::Input::mouseButtonReleased(Mouse button) {

    int scancode = static_cast<int>(button);

    if (Core::event_.type == SDL_EVENT_MOUSE_BUTTON_UP) {
        if (Core::event_.button.button == scancode)
            return true;
    }

    return false;

}

glm::vec2 fsx::Input::getMousePos() { return mousePos_; }