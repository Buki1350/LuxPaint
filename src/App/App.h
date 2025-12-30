#pragma once

#include "../Shared/Singleton/Singleton.h"
#include "../Shared/SnapshotsManager/SnapshotManager.h"
#include "../Shared/UIObjects/UIObjectsManager.h"
#include "../UI/Canvas/Canvas.h"
#include "../UI/ColorHolder/ColorHolder.h"
#include "../UI/ManagerButton/ManagerButton.h"
#include "../UI/ToolBox/ToolBox.h"
#include "../UI/ToolSizeSlider/ToolSizeSlider.h"
#include "AppData/AppData.h"

class App : public Singleton<App> {
    AppData _appData;

    void _close();
    void _update();
    void _draw();
    void _handleDebugDrawing();

public:

    UIObjectsManager UIObjectManager;
    SnapshotManager snapshotManager;
    ToolSizeSlider toolSizeSlider;
    ManagerButton managerButton;
    ColorHolder colorHolder;
    ToolBox toolBox;
    Canvas canvas;

    void init();
    void run();
    const AppData& getAppData();
    AppData& getMutableAppData();
};