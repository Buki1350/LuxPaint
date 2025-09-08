#include "App.h"

#include "Render/UIObjectManager.h"
#include "Static/FilesManager/FilesManager.h"
#include "Static/Utils/Utils.h"

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

    Utils::Init();
    UIObjectManager::Init();
    managerButton.Init();
    colorHolder.Init();
    toolBox.Init();
}

void App::Update()
{
    Utils::Update();
    Updatables::UpdateAll();
}

void App::Draw()
{
    BeginDrawing();
    ClearBackground(RAYWHITE);

    UIObjectManager::DrawAll();

    EndDrawing();
}

void App::Close() {
  int screenWidth = GetScreenWidth();
  int screenHeight = GetScreenHeight();
  int windowX = (int)GetWindowPosition().x;
  int windowY = (int)GetWindowPosition().y;

  FilesManager::SaveArray<int>("UserPreferences.dat", "windowSize",
                     {screenWidth, screenHeight});
  FilesManager::SaveArray<int>("UserPreferences.dat", "windowPosition",
                     {windowX, windowY});

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

