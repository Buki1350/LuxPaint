#pragma once

#include "UI/ColorHolder/ColorHolder.h"
#include "UI/ManagerButton/ManagerButton.h"
#include "Render/UIObjectManager.h"
#include "UI/ToolBox/ToolBox.h"
#include "raylib.h"

class App {
    UIObjectManager UIObjectManager;
    ToolBox toolBox;
    ManagerButton managerButton;
    ColorHolder colorHolder;
    void Close();

public:
    void Init();
    void Run();
    void Update();
    void Draw();
};