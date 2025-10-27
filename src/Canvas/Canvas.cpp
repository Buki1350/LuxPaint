//
// Created by lukas on 10.09.2025.
//

#include "Canvas.h"

#include "../Math/Vec2.h"
#include "../Render/UIObjectsManager.h"
#include "../StaticShared/FilesManager/FilesManager.h"
#include "../StaticShared/Utils/Utils.h"
#include "MarchingAntsSelector/MarchingAntsSelector.h"

void Canvas::Init() { _CreateBackground(); }

void Canvas::Update() {
  _HandleZoomAndDrag();
  _UpdateBackground();
  //_HandleOutline();
  _HandlePainting();
}

bool Canvas::_CanDrag() {
  return IsKeyDown(KEY_SPACE) || IsMouseButtonDown(MOUSE_MIDDLE_BUTTON);
}

void Canvas::_HandleZoomAndDrag() {
  Vec2f mouse = GetMousePosition();

  float wheel = GetMouseWheelMove();
  if (wheel != 0) {
    for (auto oImg : _oLayers) {
      Vec2f mousePos = Utils::GetMousePosition();
      Vec2f cursorOffset = mousePos - oImg->position;

      Vec2f newSize = oImg->size * (1.0f + wheel * 0.1f);
      Vec2f scale = Vec2f{newSize.x / oImg->size.x, newSize.y / oImg->size.y};

      oImg->position += Vec2f{cursorOffset.x * (1.0f - scale.x),
                              cursorOffset.y * (1.0f - scale.y)};

      oImg->size = newSize;
    }
  }

  if (_CanDrag()) {
    SetMouseCursor(MOUSE_CURSOR_RESIZE_ALL);

    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
      for (auto it = _oLayers.rbegin(); it != _oLayers.rend(); ++it) {
        UIObject *img = *it;
        if (img->CursorAbove()) {
          _oActiveImage = img;
          _dragOffset = {mouse.x - img->position.x, mouse.y - img->position.y};
          _isDragging = true;
          break;
        }
      }
    }

    if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
      _isDragging = false;
    }

    if (_isDragging && _oActiveImage) {
      Vec2f delta = {mouse.x - _dragOffset.x, mouse.y - _dragOffset.y};
      Vec2f move = delta - _oActiveImage->position;

      for (auto *img : _oLayers) {
        img->position += move;
      }
    }
  } else {
    SetMouseCursor(MOUSE_CURSOR_DEFAULT);
  }
}

void Canvas::_CreateBackground() {
  _oBackground = new UIObject();
  _oBackground->SetImage(FilesManager::LoadImage("transparent_background_medium_blend.png"));
  _oBackground->zLayer = 0;
}

void Canvas::_UpdateBackground() {
  Vec2f monitorSize = Utils::GetCurrentMonitorSize().CastTo<float>();
  _oBackground->size = {(float)monitorSize.x, (float)monitorSize.y};
  _oBackground->position = {0, 0};
}

void Canvas::_HandleOutline() {
  if (_oActiveImage) {
    MarchingAntsSelector::Instance->StartOn(_oActiveImage);
  }
  else {
    // Możesz tu ewentualnie usuwać wszystkie, jak chcesz
    // lub wywoływać StopOn() dla konkretnego
  }
}

void Canvas::_HandlePainting() {
  if (_currentTool == nullptr || !IsMouseButtonDown(0)) return;
  UIObject* hitLayer = nullptr;
  for (auto layer : _oLayers) {
    if (layer->CursorAbove()) {
      hitLayer = layer;
      break;
    }
  }

  if (!hitLayer) return;

  // ... calculating hit position
  Vec2f onTextureHitPosition = Utils::GetMousePosition() - hitLayer->position;
  _currentTool->Apply(hitLayer, onTextureHitPosition);

}

void Canvas::AddImage(Image image) {
  UIObject* uiObj = new UIObject();// UIObjectsManager::Create();
  uiObj->SetImage(image);
  uiObj->size = {(float)image.width, (float)image.height};
  uiObj->position = {100, 100};
  uiObj->zLayer = (int)_oLayers.size();
  uiObj->color = PURPLE;
  uiObj->outlineScale = 0.0f;
  _oLayers.push_back(uiObj);
}

void Canvas::AddTexture(Texture2D texture) {
  AddImage(LoadImageFromTexture(texture));
}

void Canvas::SetCurrentTool(Tool *tool) { _currentTool = tool; }
void Canvas::SetCurrentColor(Color color) { _currentColor = color; }
Color Canvas::GetCurrentColor() { return  _currentColor; }

