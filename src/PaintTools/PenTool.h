#pragma once
#include "Shared/Tool.h"

class PenTool final : public Tool {
  public:
  explicit PenTool(const std::string& name) : Tool(name) {}

  void OnClick() override;
};