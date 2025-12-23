#pragma once

#include "../Canvas/Canvas.h"
#include "../Shared/Singleton/Singleton.h"
#include "../Shared/SnapshotsManager/SnapshotManager.h"
#include "../Shared/UIObjects/UIObjectsManager.h"
#include "../UI/ColorHolder/ColorHolder.h"
#include "../UI/ManagerButton/ManagerButton.h"
#include "../UI/ToolBox/ToolBox.h"
#include "../UI/ToolSizeSlider/ToolSizeSlider.h"
#include "AppData.h"
#include "Shared/Utils/Files/utiFiles.h"

class App : public Singleton<App> {
    void Close();
    void _Update();
    void _Draw();
    void _HandleDebugDrawing();

    AppData _appData = {};
    friend void uti::files::updateAppData();

public:
    // TODO: backgroundColor;

    UIObjectsManager UIObjectManager;
    SnapshotManager snapshotManager;
    ToolSizeSlider toolSizeSlider;
    ManagerButton managerButton;
    ColorHolder colorHolder;
    ToolBox toolBox;
    Canvas canvas;

    void Init();
    void Run();
    const AppData& getAppData();
};