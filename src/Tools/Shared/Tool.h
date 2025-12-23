#pragma once
#include "../../Shared/Serializer/Serializer.h"
#include "../../Shared/UIObjects/Prefabs/Button.h"
#include "raylib.h"

#include <string>

class Tool {

  protected:
  virtual void _handleMousePressedImpl(UIObject* imageToPaint) = 0;
  virtual void _handleMouseDownImpl(UIObject* imageToPaint) = 0;
  virtual void _handleMouseReleaseImpl(UIObject* imageToPaint) = 0;
  std::string _name;
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
};
