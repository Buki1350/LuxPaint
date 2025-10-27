#pragma once

#include "Canvas/Canvas.h"
#include "Render/UIObjectsManager.h"
#include "StaticShared/Singleton/Singleton.h"
#include "UI/ColorHolder/ColorHolder.h"
#include "UI/ManagerButton/ManagerButton.h"
#include "UI/ToolBox/ToolBox.h"

class App : public Singleton<App> {
    void Close();

public:
    Canvas canvas;
    UIObjectsManager UIObjectManager;
    ManagerButton managerButton;
    ColorHolder colorHolder;
    ToolBox toolBox;

    void Init();
    void Run();
    void Update();
    void Draw();
};