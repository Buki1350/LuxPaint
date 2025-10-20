#pragma once
#include "../Shared/Tool.h"

class BrushTool final : public Tool {
public:
    explicit BrushTool(const std::string& name) : Tool(name) {}
};