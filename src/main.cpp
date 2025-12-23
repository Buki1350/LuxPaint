#include "App/App.h"

int main() {
    App::instance->Init();
    App::instance->Run();
    
    return 0;
}
