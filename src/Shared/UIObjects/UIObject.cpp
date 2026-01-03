#include "UIObject.h"

#include "../../Defines.h"
#include "../DelayedAction/DelayedAction_until.h"
#include "Shared/Utils/Colors/Colors.h"
#include "Shared/Utils/Input/Input.h"
#include "Shared/Utils/View/utiView.h"
#include "UIObjectsManager.h"
#include "raylib.h"

UIObject::UIObject() {
  text.setParent(this);
  
  new DelayedAction_until(
      []() { return UIObjectsManager::isInitialized; },
      [this]() {
          UIObjectsManager::_pendingUIObjects.push_back(this);
      }
  );
}

Vec2i UIObject::getImageSize() {
  if (_hasTexture) {
    return {_texture.width, _texture.height};
  }
  return {0, 0};
}

Vec2i UIObject::getOnImageCursorPosition() {
  if (!_hasTexture) return {-1, -1};

  Vec2f mouse = GetMousePosition();

  if (mouse.x < position.x || mouse.x > position.x + size.x ||
      mouse.y < position.y || mouse.y > position.y + size.y) {
    return {-1, -1};
      }

  Vec2f local = mouse - position;

  float texW = (float)_texture.width;
  float texH = (float)_texture.height;

  float dispW = size.x;
  float dispH = size.y;

  float scaleX, scaleY;

  if (imageStretch) {
    scaleX = texW / dispW;
    scaleY = texH / dispH;
  }
  else {
    float sx = dispW / texW;
    float sy = dispH / texH;
    float scale = fmin(sx, sy);

    float realW = texW * scale;
    float realH = texH * scale;

    float offX = (dispW - realW) * 0.5f;
    float offY = (dispH - realH) * 0.5f;

    if (local.x < offX || local.x > offX + realW ||
        local.y < offY || local.y > offY + realH)
      return {-1, -1};

    local.x -= offX;
    local.y -= offY;

    scaleX = texW / realW;
    scaleY = texH / realH;
  }

  int px = (int)(local.x * scaleX);
  int py = (int)(local.y * scaleY);

  if (px < 0 || py < 0 || px >= texW || py >= texH)
    return {-1, -1};

  return {px, py};
}


Texture &UIObject::getTexture() { return _texture; }

void UIObject::destroy() {
  Animator::terminate(this);
  UIObjectsManager::_objectsInRenderOrder.remove(this);
  delete this;
}


void UIObject::draw() {
  Vec2f finalPos = position;
  Vec2f finalSize = size;

  float smallerEdge = fmin(size.x, size.y);
  float scaleFactor = smallerEdge > 0 ? (smallerEdge / smallerEdge) : 1.0f;

  DrawRectangleRounded(
      { finalPos.x, finalPos.y, finalSize.x, finalSize.y },
      keepRoundness ? roundness * scaleFactor : roundness,
      20,
      color
  );

  if (outlineScale != 0) {
    float outlineThickness = outlineScale * uti::view::getSmallerMonitorEdge();

    Color outlineColorUsed = {};
    if (uti::colors::areColorsEqual(outlineColor, Color{0, 0, 0, 0})) {
      outlineColorUsed = Color {
        (unsigned char)(color.r * UIOBJECT_OUTLINE_DARKENING),
        (unsigned char)(color.g * UIOBJECT_OUTLINE_DARKENING),
        (unsigned char)(color.b * UIOBJECT_OUTLINE_DARKENING),
        (unsigned char)(color.a * UIOBJECT_OUTLINE_DARKENING)
      };
    } else {
      outlineColorUsed = outlineColor;
    }

    DrawRectangleRoundedLinesEx(
        { finalPos.x, finalPos.y, finalSize.x, finalSize.y },
        keepRoundness ? roundness * scaleFactor : roundness,
        20,
        outlineThickness,
        outlineColorUsed
    );
  }

  if (_hasTexture) {
    float margin = imageMarginScale * uti::view::getSmallerMonitorEdge();

    if (imageStretch) {
      DrawTexturePro(
          _texture,
          { 0, 0, (float)_texture.width, (float)_texture.height },
          {
              finalPos.x + margin,
              finalPos.y + margin,
              finalSize.x - 2*margin,
              finalSize.y - 2*margin
          },
          { 0, 0 },
          0.0f,
          Fade(WHITE, imageAlpha)
      );
    } else {
      float scaleX = finalSize.x / (float)_texture.width;
      float scaleY = finalSize.y / (float)_texture.height;
      float scale = fmin(scaleX, scaleY);

      float destW = _texture.width * scale;
      float destH = _texture.height * scale;
      DrawTexturePro(
          _texture,
          { 0, 0, (float)_texture.width, (float)_texture.height },
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
  }

  text.draw();
}

void UIObject::setZLayer(int newZLayer) {
  UIObjectsManager::_objectsInRenderOrder.remove(this);
  this->_zLayer = newZLayer;
  UIObjectsManager::_objectsInRenderOrder.push_back(this);
  UIObjectsManager::_updateRenderOrderList();
}

int UIObject::getZLayer() const { return this->_zLayer; }

bool UIObject::cursorAbove() const {
  return
      isActive &&
      GetMousePosition().x >= position.x &&
      GetMousePosition().x <= position.x + size.x &&
      GetMousePosition().y >= position.y &&
      GetMousePosition().y <= position.y + size.y;
}

bool UIObject::clicked() const {
  return isActive && cursorAbove() && uti::input::mouseReleased();
}

bool UIObject::clickedButNotThis() {
  return isActive && !cursorAbove() && uti::input::mouseReleased();
}

bool UIObject::pressed() const {
  return isActive && cursorAbove() && IsMouseButtonPressed(0);
}

void UIObject::setImage(const Texture &texture) {
  _texture = texture;
  _hasTexture = true;
}

void UIObject::setImage(const Image &image) {
  _image = ImageCopy(image);
  _texture = LoadTextureFromImage(_image);
  _hasTexture = true;
  _hasImage = true;
}

void UIObject::updateTexture() {
  if (_hasImage && _hasTexture) {
    ::UpdateTexture(_texture, _image.data);
  }
}

Image UIObject::getImage() {
  return _image;
}

