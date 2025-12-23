#pragma once
#include "raylib.h"
#include <string>

class UIObject;

enum TextWrapType {
  EXPAND,
  CUT,
};

class Text {
public:
  UIObject* uiObject_parent = nullptr;
  std::string value = "";
  int fontSize = 20;
  int margin = 0;
  Color textColor = BLACK;
  bool center = false;

  float getPixelWidth();
  void setParent(UIObject *parent);
  void wrapToWidth(TextWrapType wrapType);
  void draw();

  void operator=(const char *other) { value = other; }
  Text &operator=(const std::string & other) { value = other; return *this; }
};