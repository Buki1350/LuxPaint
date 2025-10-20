#pragma once
#include "../../src/Render/Prefabs/Button.h"
#include "../Shared/Tool.h"
#include <raylib.h>

class ToolSet {
    public:
    Button* button;
    std::vector<Tool*> tools;

    ToolSet(const std::string &name, const std::vector<Tool*> &tools);
};