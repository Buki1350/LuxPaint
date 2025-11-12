#pragma once
#include "../../Render/Prefabs/Button.h"
#include "../../StaticShared/FilesManager/FilesManager.h"
#include "raylib.h"

#include <string>

class Tool {
  friend class Canvas; // only canvas can use tools

  protected:
  std::string _name;
  virtual void _HandleMousePressed(UIObject* imageToPaint, Vec2f vec2) = 0;
  virtual void _HandleMouseDown(UIObject* imageToPaint, Vec2f vec2) = 0;
  virtual void _HandleMouseRelease(UIObject* imageToPaint, Vec2f vec2) = 0;

  public:
  Texture2D icon;
  Tool(const std::string &name);
};
