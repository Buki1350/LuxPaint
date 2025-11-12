#include "App.h"
#include "Automatition/Initializables/Initializables.h"
#include "Render/UIObjectsManager.h"
#include "StaticShared/Animator/Animator.h"
#include "StaticShared/FilesManager/FilesManager.h"

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
    managerButton.Init();
    colorHolder.Init();
    toolBox.Init();
    canvas.Init();
}

void App::Update()
{
    UpdatablesManager::UpdateAll();
}

void App::Draw()
{
    BeginDrawing();
    ClearBackground(GRAY);
    UIObjectsManager::DrawAll();
    EndDrawing();
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
        Update();
        Draw();
    }
    Close();
}

