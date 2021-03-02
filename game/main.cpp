#include "glApi.hpp"
#include "GameApplication.hpp"

int main(int argc, char * argv[]) {
    GameApplication * application = new GameApplication(500, 500);
    application->setCallbacks();
    application->mainLoop();
    delete application;

}