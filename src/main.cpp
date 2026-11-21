#include "fsx-lib/fsx.hpp"
using namespace fsx;

int main() { 

    Utils::setLogLevel(Utils::LogLevel::VERBOSE);

    Window window;
    window.setTitle("poggers");

    while (Core::running()) {

        while (Core::pollEvents());

    }

    return 0;

}