#pragma once
#include "../../src/Shared/UIObjects/Prefabs/Button.h"
#include "../Shared/Tool.h"
#include <raylib.h>

class ToolSet {
    public:
    Button* button;
    std::vector<Tool*> tools;
    std::string name;

    ToolSet(const std::string &name, const std::vector<Tool*> &tools);
};