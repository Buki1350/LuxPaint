#pragma once
#include "../../Updatables/Updatable.h"
#include "../../src/Render/UIObject.h"
#include "../Shared/Tool.h"
#include <raylib.h>

class ToolSet {
    public:
    UIObject* uiObject;
    std::vector<Tool> tools;

    ToolSet(const std::string &name, const std::vector<Tool> &tools);
};