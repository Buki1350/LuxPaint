#pragma once
#include "../../Render/Prefabs/Button.h"
#include "../../StaticShared/FilesManager/FilesManager.h"
#include "raylib.h"

#include <string>

class Tool {
  protected:
  std::string toolName;

  public:
  Texture2D icon;
  Tool(const std::string &name);
  virtual void HandleClick(UIObject* imageToPaint, Vec2f vec2) = 0;
  virtual void HandleRelease(UIObject* imageToPaint, Vec2f vec2) = 0;
};
