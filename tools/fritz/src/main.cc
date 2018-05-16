#include <exception>
#include "Fritz.h"
#include "Global.h"

int main(int argc, char *argv[]) {
    Fritz* fritz = new Fritz();
    try {
        fritz->initialize(argc, argv);
        fritz->processEventLoop();
    }
    catch(std::exception& e) {
        Global::abort("Unknown", e.what());
        fritz->finalize();
        return 1;
    }
    fritz->finalize();
    // FIXME: delete fritz
    return 0;
}
