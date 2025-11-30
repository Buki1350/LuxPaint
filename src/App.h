#pragma once

#include "Canvas/Canvas.h"
#include "Shared/Singleton/Singleton.h"
#include "UI/ColorHolder/ColorHolder.h"
#include "UI/ManagerButton/ManagerButton.h"
#include "UI/ToolBox/ToolBox.h"
#include "UI/ToolSizeSlider/ToolSizeSlider.h"
#include "Shared/UIObjects/UIObjectsManager.h"

class App : public Singleton<App> {
    void Close();

    void _Update();
    void _Draw();
    void _HandleDebugDrawing();
public:
    Canvas canvas;
    UIObjectsManager UIObjectManager;
    ToolSizeSlider toolSizeSlider;
    ManagerButton managerButton;
    ColorHolder colorHolder;
    ToolBox toolBox;

    void Init();
    void Run();
};