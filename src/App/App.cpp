#include "App.h"
#include "../Automatition/Initializables/Initializables.h"
#include "../Debug/Debug.h"
#include "../Shared/Serializer/Serializer.h"
#include "../Shared/UIObjects/UIObjectsManager.h"
#include "Shared/Utils/Files/utiFiles.h"

static App _;

void App::Init()
{
    SetConfigFlags(FLAG_WINDOW_TRANSPARENT | FLAG_WINDOW_RESIZABLE | FLAG_MSAA_4X_HINT);

    std::vector<int> windowSize = Serializer::LoadArray<int>("UserPreferences.dat", "windowSize");
    if (windowSize.empty()) windowSize = {800, 600};
    InitWindow(windowSize[0], windowSize[1], "Lux Paint");

    std::vector<int> windowPosition = Serializer::LoadArray<int>("UserPreferences.dat", "windowPosition");
    if (!windowPosition.empty()) SetWindowPosition(windowPosition[0], windowPosition[1]);

    SetTargetFPS(60);
    UIObjectsManager::Init();
    Initializables::InitAll();
    toolSizeSlider.init();
    managerButton.init();
    colorHolder.init();
    toolBox.init();
    canvas.init();
}

void App::_Update()
{
  uti::files::updateAppData();
  UpdatablesManager::UpdateAll();
}

void App::_Draw() {
    BeginDrawing();
    ClearBackground(Color{66, 66, 66, 127});
    UIObjectsManager::DrawAll();
#ifdef DEBUG
    _HandleDebugDrawing();
#endif
  EndDrawing();
}

void App::_HandleDebugDrawing() {
    Debug::DrawAll();
}

void App::Close() {
  int screenWidth = GetScreenWidth();
  int screenHeight = GetScreenHeight();
  int windowX = (int)GetWindowPosition().x;
  int windowY = (int)GetWindowPosition().y;

  Serializer::SaveArray<int>("UserPreferences.dat", "windowSize", {screenWidth, screenHeight});
  Serializer::SaveArray<int>("UserPreferences.dat", "windowPosition", {windowX, windowY});
  CloseWindow();
}

void App::Run() {
  while (!WindowShouldClose()) {
    _Update();
    _Draw();
  }
  Close();
}

const AppData& App::getAppData() {
  return _appData;
}

