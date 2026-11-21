#include "fsx-lib/fsx.hpp"
using namespace fsx;

Window window;

void init() {

    Utils::setLogLevel(Utils::LogLevel::VERBOSE);
    window.setTitle("poggers");

}

void loop() {

    

}
    
int main() { return fsx::Core::run(init, loop); }