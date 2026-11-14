#include "fsx-lib/fsx.hpp"

fsx::Window window;

void fsx::init() {

    window.title = "pog";
    window.open();

}

void fsx::loop() {

    if (fsx::Input::keyHeld(fsx::Input::Keys::KEY_UP))
        std::println("UP");
    
    if (fsx::Input::keyHeld(fsx::Input::Keys::KEY_LEFT))
        std::println("LEFT");

    if (fsx::Input::keyHeld(fsx::Input::Keys::KEY_DOWN))
        std::println("DOWN");

    if (fsx::Input::keyHeld(fsx::Input::Keys::KEY_RIGHT))
        std::println("RIGHT");

}

void fsx::exit() {



}

int main() {

    fsx::run();

}