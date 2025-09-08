#pragma once
#include "../Static/Animator/Animator.h"
#include "Text.h"
#include "raylib.h"
#include <string>

class UIObject {
  Texture2D   _image = {};
  bool        _hasImage = false;

public:
  std::string name = "";
  Vector2     position {0,0};
  Vector2     size {0,0};
  Vector2     animOffsetPos {0,0};
  Vector2     animOffsetSize {0,0};
  Color       color {0,0,0};
  float       roundness = 0;
  float       imageAlpha = 1;
  int         zLayer = 0;
  bool        isActive = true;
  bool        keepRoundness = true;
  Text        text;

  UIObject();
  void Draw();
  bool IsCursorAbove() const;
  bool IsClicked() const;
  void SetImage(const Texture &texture);
  void SetImage(const Image &image);
  Vector2 GetAnimatedPosition();
  Vector2 GetAnimatedSize();
};