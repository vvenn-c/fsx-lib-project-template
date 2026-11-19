#include "fsx-lib/fsx.hpp"
using namespace fsx;
using namespace glm;

Window window;

// TODO: Window::openWindows need not be public

void init() {

    Utils::setLogLevel(Utils::LogLevel::VERBOSE);
    window.setTitle("poggers");

}

void loop() {
    
    

}

int main() { return Core::run(init, loop); }