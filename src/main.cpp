#include "fsx-lib/fsx.hpp"

Window window;

void init() {

    Utils::setLogLevel(Utils::LogLevel::VERBOSE);
    window.setTitle("poggers");

}

void loop() {
    
    

}

int main() { return Core::run(init, loop); }