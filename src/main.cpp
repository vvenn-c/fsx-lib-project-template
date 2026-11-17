#include "fsx-lib/fsx.hpp"
using namespace fsx;

Window window;

void fsx::init() {

    window.title = "pog";
    window.open();

}

void fsx::loop() {

    if (Input::keyPressed(Input::Key::F))
        Utils::toggle(&window.borderless);

}

void fsx::exit() {



}

int main() {

    fsx::run();

}