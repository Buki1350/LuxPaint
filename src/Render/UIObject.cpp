#include "UIObject.h"

#include "UIObjectManager.h"
#include "raylib.h"



UIObject::UIObject() { text.SetParent(this); }

Vector2 UIObject::GetAnimatedPosition() { return { position.x + animOffsetPos.x, position.y + animOffsetPos.y }; }
Vector2 UIObject::GetAnimatedSize() { return { size.x + animOffsetSize.x, size.y + animOffsetSize.y }; }

void UIObject::Draw() {
  Vector2 finalPos = GetAnimatedPosition();
  Vector2 finalSize = GetAnimatedSize();
  float smallerEdge = size.x > size.y? size.x : size.y;
  float smallerOffsetEdge = animOffsetSize.x > animOffsetSize.y? animOffsetSize.x : animOffsetSize.x;
  float scaleFactor = smallerEdge / (smallerEdge + smallerOffsetEdge);

  DrawRectangleRounded(
      { finalPos.x, finalPos.y, finalSize.x, finalSize.y },
      keepRoundness? roundness * scaleFactor : roundness,
      20,
      color
  );

  if (_hasImage) {
    float scale = 1.0f;

    if (_image.width > size.x || _image.height > size.y) {
      float scaleX = size.x / (float)_image.width;
      float scaleY = size.y / (float)_image.height;
      scale = (float)fmin(scaleX, scaleY) * 0.8f;
    }

    float destW = _image.width * scale;
    float destH = _image.height * scale;

    DrawTexturePro(
        _image,
        {0, 0, (float)_image.width, (float)_image.height},
        { position.x + (size.x - destW)/2,
          position.y + (size.y - destH)/2,
          destW, destH },
        {0,0},
        0.0f,
        Fade(WHITE, imageAlpha)
    );
  }

  text.Draw();
}

bool UIObject::IsCursorAbove() const {
  Vector2 finalPos = { position.x + animOffsetPos.x, position.y + animOffsetPos.y };
  Vector2 finalSize = { size.x + animOffsetSize.x, size.y + animOffsetSize.y };

  return
      GetMousePosition().x >= finalPos.x &&
      GetMousePosition().x <= finalPos.x + finalSize.x &&
      GetMousePosition().y >= finalPos.y &&
      GetMousePosition().y <= finalPos.y + finalSize.y;
}

bool UIObject::IsClicked() const {
  return IsCursorAbove() && IsMouseButtonPressed(0);
}

void UIObject::SetImage(const Texture &texture) {
  _image = texture;
  _hasImage = true;
}

void UIObject::SetImage(const Image &image) {
  Texture2D texture = LoadTextureFromImage(image);
  _image = texture;
  _hasImage = true;
}