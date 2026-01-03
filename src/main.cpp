#include "App/App.h"
#include "Debug/Tests/Tests.h"

int main() {
#ifdef DEBUG
    if (!Tests::validate()) { return 1; }
#endif
    App::instance().init();
    App::instance().run();
    
    return 0;
}
