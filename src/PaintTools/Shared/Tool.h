#pragma once
#include "../../Render/Prefabs/Button.h"
#include "../../StaticShared/FilesManager/FilesManager.h"
#include "raylib.h"

#include <string>

class Tool {
  protected:
  std::string toolName;

  public:
  Button button;
  Tool(const std::string &name);
};
