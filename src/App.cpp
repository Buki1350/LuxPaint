#include "App.h"
#include "Automatition/Initializables/Initializables.h"
#include "Debug/Debug.h"
#include "Shared/Animator/Animator.h"
#include "Shared/FilesManager/FilesManager.h"
#include "Shared/UIObjects/UIObjectsManager.h"

static App _;

void App::Init()
{
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    SetConfigFlags(FLAG_MSAA_4X_HINT);

    std::vector<int> windowSize = FilesManager::LoadArray<int>("UserPreferences.dat", "windowSize");
    if (windowSize.empty()) windowSize = {800, 600};
    InitWindow(windowSize[0], windowSize[1], "Lux Paint");

    std::vector<int> windowPosition = FilesManager::LoadArray<int>("UserPreferences.dat", "windowPosition");
    if (!windowPosition.empty()) SetWindowPosition(windowPosition[0], windowPosition[1]);

    SetTargetFPS(60);
    Utils::Files::SetDefaultFont();
    UIObjectsManager::Init();
    Initializables::InitAll();
    toolSizeSlider.Init();
    managerButton.Init();
    colorHolder.Init();
    toolBox.Init();
    canvas.Init();
}

void App::_Update()
{
    UpdatablesManager::UpdateAll();
}

void App::_Draw() {
  BeginDrawing();
  ClearBackground(GRAY);
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

  FilesManager::SaveArray<int>("UserPreferences.dat", "windowSize", {screenWidth, screenHeight});
  FilesManager::SaveArray<int>("UserPreferences.dat", "windowPosition", {windowX, windowY});
  CloseWindow();
}

void App::Run()
{
    while (!WindowShouldClose())
    {
        _Update();
        _Draw();
    }
    Close();
}

