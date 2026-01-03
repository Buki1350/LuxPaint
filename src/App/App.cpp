#include "App.h"
#include "../Automatition/Initializables/Initializables.h"
#include "../Debug/Debug.h"
#include "../Shared/Serializer/Serializer.h"
#include "../Shared/UIObjects/UIObjectsManager.h"

void App::init()
{
  SetConfigFlags(FLAG_WINDOW_TRANSPARENT | FLAG_WINDOW_RESIZABLE | FLAG_MSAA_4X_HINT);
  SetTargetFPS(60);

  std::vector<int> windowSize = Serializer::loadArray<int>("UserPreferences.dat", "windowSize");
  if (windowSize.empty()) windowSize = {800, 600};
  InitWindow(windowSize[0], windowSize[1], "Lux Paint");

  std::vector<int> windowPosition = Serializer::loadArray<int>("UserPreferences.dat", "windowPosition");
  if (!windowPosition.empty()) SetWindowPosition(windowPosition[0], windowPosition[1]);

  _appData.init();

  UIObjectsManager::init();
  Initializables::initAll();
}

void App::update()
{
  _appData.updateValues();
  UpdatablesManager::updateAll();
}

void App::draw() {
    BeginDrawing();
    ClearBackground(_appData.appBackgroundColor);//127});
    UIObjectsManager::DrawAll();
#ifdef DEBUG
    handleDebugDrawing();
#endif
  EndDrawing();
}

void App::handleDebugDrawing() {
    Debug::DrawAll();
}

void App::close() {
  _appData.saveValues();
  CloseWindow();
}

void App::run() {
  while (!WindowShouldClose()) {
    update();
    draw();
  }
  close();
}

const AppData &App::getAppData() { return _appData; }
AppData &App::getMutableAppData() { return _appData; }

