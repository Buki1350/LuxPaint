#pragma once

#include "Canvas/Canvas.h"
#include "Render/UIObjectManager.h"
#include "StaticShared/Singleton/Singleton.h"
#include "UI/ColorHolder/ColorHolder.h"
#include "UI/ManagerButton/ManagerButton.h"
#include "UI/ToolBox/ToolBox.h"
#include "raylib.h"

class App : public Singleton<App> {
    void Close();

public:
    Canvas canvas;
    UIObjectManager UIObjectManager;
    ManagerButton managerButton;
    ColorHolder colorHolder;
    ToolBox toolBox;

    void Init();
    void Run();
    void Update();
    void Draw();
};