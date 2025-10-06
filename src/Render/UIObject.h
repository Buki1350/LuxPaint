#pragma once
#include "../Defines.h"
#include "../Math/Vec2.h"
#include "Text.h"
#include "raylib.h"
#include <string>

class UIObject {
  Texture2D   _texture = {};
  bool        _hasTexture = false;

public:
  std::string   name = "";
  Vec2f         position {0,0};
  Vec2f         size {0,0};
  Color         color {0,0,0};
  float         roundness = 0;
  float         imageAlpha = 1;
  float         imageMarginScale = 0;
  float         outlineScale = UIOBJECT_OUTLINE_SCALE;
  int           zLayer = 0;
  Text          text;
  bool          isActive = true;
  bool          keepRoundness = true;

  UIObject();
  virtual ~UIObject() = default;
  virtual void Draw();
  bool CursorAbove() const;
  bool Clicked() const;
  void SetImage(const Texture &texture);
  void SetImage(const Image &image);
  Vec2i GetImageSize();
  Texture & GetTexture();
};

// animator utility
enum UIObjectParameter {
  NONE, // for reset
  POSITION,
  SIZE,
  COLOR,
  IMAGE_ALPHA,
  ROUNDNESS
};