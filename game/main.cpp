#include "glApi.hpp"
#include "application/GameApplication.hpp"

int main() {

    srand(time(nullptr));

    auto *application = new GameApplication(500, 500);
    application->setCallbacks();
    application->mainLoop();
    delete application;
}