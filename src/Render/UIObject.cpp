#include "UIObject.h"

#include "../Defines.h"
#include "../StaticShared/Utils/Utils.h"
#include "UIObjectsManager.h"
#include "raylib.h"

UIObject::UIObject() {
  text.SetParent(this);
  UIObjectsManager::objects.push_back(this);
}

UIObject::~UIObject() {
  Animator::Terminate(this);
  for (auto it = UIObjectsManager::objects.begin(); it != UIObjectsManager::objects.end(); ++it) {
    if (*it == this) {
      UIObjectsManager::objects.erase(it);
      break;
    }
  }

  if (auto up = dynamic_cast<Updatable*>(this)) {
    auto &vec = UpdatablesManager::updatables;
    vec.erase(std::remove(vec.begin(), vec.end(), up), vec.end());
  }

  delete this;
}

Vec2i UIObject::GetImageSize() {
  if (_hasTexture) {
    return {_texture.width, _texture.height};
  }
  return {0, 0};
}

Texture &UIObject::GetTexture() {
  return _texture;
}

void UIObject::Draw() {
  Vec2f finalPos = position;
  Vec2f finalSize = size;

  // jeśli chcesz zachować proporcję zaokrągleń względem rozmiaru
  float smallerEdge = fmin(size.x, size.y);
  float scaleFactor = smallerEdge > 0 ? (smallerEdge / smallerEdge) : 1.0f; // teraz zawsze 1.0f

  DrawRectangleRounded(
      { finalPos.x, finalPos.y, finalSize.x, finalSize.y },
      keepRoundness ? roundness * scaleFactor : roundness,
      20,
      color
  );

  if (outlineScale != 0) {
    float outlineThickness = outlineScale * Utils::GetSmallerMonitorEdge();
    Color outlineColor {
      (unsigned char)(color.r * UIOBJECT_OUTLINE_DARKENING),
      (unsigned char)(color.g * UIOBJECT_OUTLINE_DARKENING),
      (unsigned char)(color.b * UIOBJECT_OUTLINE_DARKENING),
      (unsigned char)(color.a * UIOBJECT_OUTLINE_DARKENING)
  };

    DrawRectangleRoundedLinesEx(
        { finalPos.x, finalPos.y, finalSize.x, finalSize.y },
        keepRoundness ? roundness * scaleFactor : roundness,
        20,
        outlineThickness,
        outlineColor
    );
  }

  if (_hasTexture) {
    float scaleX = finalSize.x / (float)_texture.width;
    float scaleY = finalSize.y / (float)_texture.height;
    float scale = fmin(scaleX, scaleY);
    float margin = imageMarginScale * Utils::GetSmallerMonitorEdge();

    float destW = _texture.width * scale;
    float destH = _texture.height * scale;

    DrawTexturePro(
        _texture,
        { 0, 0, (float)_texture.width, (float)_texture.height }, // zawsze cały obraz
        {
            finalPos.x + margin,
            finalPos.y + margin,
            destW - 2 * margin,
            destH - 2 * margin
        },
        { 0, 0 },
        0.0f,
        Fade(WHITE, imageAlpha)
    );
  }
  text.Draw();
}

bool UIObject::CursorAbove() const {
  return
      isActive &&
      GetMousePosition().x >= position.x &&
      GetMousePosition().x <= position.x + size.x &&
      GetMousePosition().y >= position.y &&
      GetMousePosition().y <= position.y + size.y;
}

bool UIObject::Clicked() const {
  return isActive && CursorAbove() && Utils::MouseClicked();
}

bool UIObject::ClickReleased() const {
  return Clicked();
}

bool UIObject::Pressed() const {
  return isActive && CursorAbove() && IsMouseButtonPressed(0);
}

void UIObject::SetImage(const Texture &texture) {
  _texture = texture;
  _hasTexture = true;
}

void UIObject::SetImage(const Image &image) {
  _image = ImageCopy(image);
  _texture = LoadTextureFromImage(_image);
  _hasTexture = true;
  _hasImage = true;
}

void UIObject::UpdateTexture() {
  if (_hasImage && _hasTexture) {
    ::UpdateTexture(_texture, _image.data);
  }
}
Image UIObject::GetImage() {
  return _image;
}

