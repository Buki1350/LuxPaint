#include "App.h"
#include "../Automatition/Initializables/Initializables.h"
#include "../Debug/Debug.h"
#include "../Shared/Serializer/Serializer.h"
#include "../Shared/UIObjects/UIObjectsManager.h"

void App::init()
{
  SetConfigFlags(FLAG_WINDOW_TRANSPARENT | FLAG_WINDOW_RESIZABLE | FLAG_MSAA_4X_HINT);
  SetTargetFPS(60);

  std::vector<int> windowSize = Serializer::LoadArray<int>("UserPreferences.dat", "windowSize");
  if (windowSize.empty()) windowSize = {800, 600};
  InitWindow(windowSize[0], windowSize[1], "Lux Paint");

  std::vector<int> windowPosition = Serializer::LoadArray<int>("UserPreferences.dat", "windowPosition");
  if (!windowPosition.empty()) SetWindowPosition(windowPosition[0], windowPosition[1]);

  _appData._init();

  UIObjectsManager::Init();
  Initializables::InitAll();
  toolSizeSlider.init();
  managerButton.init();
  colorHolder.init();
  toolBox.init();
  canvas.init();
}

void App::_update()
{
  _appData._updateValues();
  UpdatablesManager::updateAll();
}

void App::_draw() {
    BeginDrawing();
    ClearBackground(_appData.appBackgroundColor);//127});
    UIObjectsManager::DrawAll();
#ifdef DEBUG
    _handleDebugDrawing();
#endif
  EndDrawing();
}

void App::_handleDebugDrawing() {
    Debug::DrawAll();
}

void App::_close() {
  int screenWidth = GetScreenWidth();
  int screenHeight = GetScreenHeight();
  int windowX = (int)GetWindowPosition().x;
  int windowY = (int)GetWindowPosition().y;

  Serializer::SaveArray<int>("UserPreferences.dat", "windowSize", {screenWidth, screenHeight});
  Serializer::SaveArray<int>("UserPreferences.dat", "windowPosition", {windowX, windowY});
  CloseWindow();
}

void App::run() {
  while (!WindowShouldClose()) {
    _update();
    _draw();
  }
  _close();
}

const AppData &App::getAppData() { return _appData; }
AppData &App::getMutableAppData() { return _appData; }

