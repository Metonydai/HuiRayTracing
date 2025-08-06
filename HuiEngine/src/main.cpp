// Sample by Sascha Willems
// Contact: webmaster@saschawillems.de
#include "vk_engine.h"

int main() {
    ComputeShaderApplication app;

    try {
        app.run();
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
