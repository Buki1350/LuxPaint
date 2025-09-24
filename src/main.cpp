#include "App.h"

int main() {
    App::Instance->Init();
    App::Instance->Run();
    
    return 0;
}
