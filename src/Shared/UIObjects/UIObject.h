#pragma once
#include "../../Defines.h"
#include "../../Math/Vec2.h"
#include "Text.h"
#include "raylib.h"
#include <string>

class UIObject {
  int         _zLayer = 0;

  bool        textFittingEnabled = false;
  float       textFittingSeparatorScale = 0.005f;

  bool        _hasTexture = false;
  Image       _image;
  Texture2D   _texture = {};
  bool        _hasImage = false;

public:
  // ... core
  bool          isActive = true;
  Text          text;
  std::string   name = "";
  // .. transform
  Vec2f         position {0,0};
  Vec2f         size {0,0};
  // ... visual
  Color         color {0, 0, 0, 0};
  float         roundness = 0;
  float         outlineScale = UIOBJECT_OUTLINE_SCALE;
  Color         outlineColor = {0, 0, 0, 0};
  bool          keepRoundness = true;
  // ... images
  bool          imageStretch = false;
  float         imageAlpha = 1;
  float         imageMarginScale = 0;

  UIObject();
  virtual void draw();
  void setZLayer(int newZLayer);
  int getZLayer() const;
  bool cursorAbove() const;
  bool clicked() const;
  bool clickedButNotThis();
  bool pressed() const;
  void setImage(const Texture &texture);
  void setImage(const Image &image);
  void updateTexture();
  Image getImage();
  Vec2i getImageSize();
  Vec2i getOnImageCursorPosition();
  Texture &getTexture();
  virtual void destroy();
};

// ... animator utility
enum UIObjectParameter {
  ANIM_NONE, // ... for reset
  ANIM_POSITION,
  ANIM_SIZE,
  ANIM_COLOR,
  ANIM_OUTLINE,
  ANIM_IMAGE_ALPHA,
  ANIM_ROUNDNESS
};