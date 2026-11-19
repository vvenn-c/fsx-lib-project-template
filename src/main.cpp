#include "fsx-lib/fsx.hpp"
using namespace fsx;
using namespace glm;

Window window;

void fsx::init() {

    Utils::setLogLevel(Utils::LogLevel::VERBOSE);
    window.setTitle("poggers");

}

void fsx::loop() {
    


}

void fsx::exit() {



}

int main() { return Core::run(); }