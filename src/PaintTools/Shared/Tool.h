#pragma once
#include "../../Render/UIObject.h"
#include "../../StaticShared/FilesManager/FilesManager.h"
#include "raylib.h"

#include <string>

class Tool {
  protected:
  UIObject toolButton;
  std::string toolName;

  public:
  Tool(const std::string &name);
  virtual void OnClick();
};
