#include "glApi.hpp"
#include "GameApplication.hpp"
#include "Asset_Designs/SimpleApp.hpp"

int main(int argc, char * argv[]) {
//    GameApplication * application = new GameApplication(500, 500);
//    application->setCallbacks();
//    application->mainLoop();
//    delete application;

    SimpleApp * application = new SimpleApp(500, 500);
    application->setCallbacks();
    application->mainLoop();
    delete application;
}