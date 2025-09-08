#pragma once
#include "raylib.h"
#include <string>

class UIObject;

class Text {
public:
  UIObject* uiObject_parent = nullptr;
  std::string value = "";
  int fontSize = 20;
  int margin = 0;
  Color textColor = BLACK;
  bool center = false;

  void SetParent(UIObject *parent);
  void Draw();

  void operator =(const char* other) {
    value = other;
  }
};