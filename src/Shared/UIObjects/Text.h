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

  float GetPixelWidth();
  void SetParent(UIObject *parent);
  void Draw();

  void operator=(const char *other) { value = other; }
  Text &operator=(const std::string & other) { value = other; return *this; }
};