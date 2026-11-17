#include "fsx-lib/fsx.hpp"
using namespace fsx;

Window window;

void fsx::init() {

    window.title = "pog";
    window.open();

}

void fsx::loop() {

    auto pos = Input::getMousePos();
    std::println("{}, {}", pos.x, pos.y);

}

void fsx::exit() {



}

int main() {

    fsx::run();

}