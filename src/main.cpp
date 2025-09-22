#include "App.h"
#include "StaticShared/FilesManager/FilesManager.h"
#include <iostream>
#include <vector>

int main() {
    App::Instance->Init();
    App::Instance->Run();
    
    return 0;
}
