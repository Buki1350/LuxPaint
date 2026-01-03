#pragma once
#include "../../Shared/Serializer/Serializer.h"
#include "../../Shared/UIObjects/Prefabs/Button.h"
#include "raylib.h"

#include <string>

class Tool {
  std::string _name;
  protected:
  virtual void handleMousePressedImpl(UIObject* imageToPaint) = 0;
  virtual void handleMouseDownImpl(UIObject* imageToPaint) = 0;
  virtual void handleMouseReleaseImpl(UIObject* imageToPaint) = 0;
  int size = 0;

  public:
  Texture2D icon;

  void handleMousePressed(UIObject * ui_object);
  void handleMouseDown(UIObject * ui_object);
  void handleMouseRelease(UIObject * ui_object);

  virtual bool canSizeBeChanged() const = 0;
  void setSize(int val);
  int getSize() { return size; }

  Tool(const std::string &name);
  virtual ~Tool() = default;
};
