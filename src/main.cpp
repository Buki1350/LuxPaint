#include "App/App.h"

int main() {
    App::instance().init();
    App::instance().run();
    
    return 0;
}
