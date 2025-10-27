#pragma once
#include "../Defines.h"
#include "../Math/Vec2.h"
#include "Text.h"
#include "raylib.h"
#include <string>

class UIObject {
  Texture2D   _texture = {};
  bool        _hasTexture = false;
  Image       _image;
  bool        _hasImage = false;

public:
  std::string   name = "";
  Vec2f         position {0,0};
  Vec2f         size {0,0};
  Color         color {};
  float         roundness = 0;
  float         imageAlpha = 1;
  float         imageMarginScale = 0;
  float         outlineScale = UIOBJECT_OUTLINE_SCALE;
  int           zLayer = 0;
  Text          text;
  bool          isActive = true;
  bool          keepRoundness = true;

  UIObject();
  ~UIObject() = default;
  virtual void Draw();
  bool CursorAbove() const;
  bool Clicked() const;
  bool ClickedButNotThis();
  bool Pressed() const;
  void SetImage(const Texture &texture);
  void SetImage(const Image &image);
  void UpdateTexture();
  Image GetImage();
  Vec2i GetImageSize();
  Texture &GetTexture();
  virtual void Destroy();
};

// ... animator utility
enum UIObjectParameter {
  NONE, // ... for reset
  POSITION,
  SIZE,
  COLOR,
  IMAGE_ALPHA,
  ROUNDNESS
};