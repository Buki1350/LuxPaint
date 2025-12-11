#pragma once
#include "../../Defines.h"
#include "../../Math/Vec2.h"
#include "Text.h"
#include "raylib.h"
#include <string>

class UIObject {
  Texture2D   _texture = {};
  bool        _hasTexture = false;
  Image       _image;
  bool        _hasImage = false;
  bool        textFittingEnabled = false;
  float       textFittingSeparatorScale = 0.005f;
  int         _zLayer = 0;

public:
  bool          isActive = true;
  std::string   name = "";
  Vec2f         position {0,0};
  Vec2f         size {0,0};
  Color         color {};
  float         roundness = 0;
  Text          text;
  // ... style
  float         outlineScale = UIOBJECT_OUTLINE_SCALE;
  bool          keepRoundness = true;
  // ... images
  bool          imageStretch = false;
  float         imageAlpha = 1;
  float         imageMarginScale = 0;

  UIObject();
  virtual void Draw();
  void SetZLayer(int newZLayer);
  int GetZLayer();
  bool CursorAbove() const;
  bool Clicked() const;
  bool ClickedButNotThis();
  bool Pressed() const;
  void SetImage(const Texture &texture);
  void SetImage(const Image &image);
  void UpdateTexture();
  Image GetImage();
  Vec2i GetImageSize();
  Vec2i GetOnImageCursorPosition();
  Texture &GetTexture();
  virtual void Destroy();
};

// ... animator utility
enum UIObjectParameter {
  NONE, // ... for reset
  POSITION,
  SIZE,
  COLOR,
  OUTLINE,
  IMAGE_ALPHA,
  ROUNDNESS
};