#pragma once
#include "../../Shared/FilesManager/FilesManager.h"
#include "../../Shared/UIObjects/Prefabs/Button.h"
#include "raylib.h"

#include <string>

class Tool {
  friend class Canvas; // only canvas can use tools

  protected:
  std::string _name;
  int size = 0;
  virtual void HandleMousePressed(UIObject* imageToPaint) = 0;
  virtual void HandleMouseDown(UIObject* imageToPaint) = 0;
  virtual void HandleMouseRelease(UIObject* imageToPaint) = 0;

  public:
  Texture2D icon;

  virtual bool CanSizeBeChanged() const = 0;
  void SetSize(float val);
  int GetSize() { return size; }

  Tool(const std::string &name);
};
