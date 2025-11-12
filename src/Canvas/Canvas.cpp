//
// Created by lukas on 10.09.2025.
//

#include "Canvas.h"

#include "../App.h"
#include "../Math/Vec2.h"
#include "../StaticShared/DelayedAction/DelayedAction_until.h"
#include "../StaticShared/FilesManager/FilesManager.h"
#include "../StaticShared/Utils/Utils.h"
#include "Selectors/MarchingAntsSelector.h"

std::vector<UIObject *> &Canvas::GetImages() { return _oLayers; }
void Canvas::SetCurrentColor(Color color) { _currentColor = color; }
Color Canvas::GetCurrentColor() { return _currentColor; }
void Canvas::SetCurrentTool(Tool *tool) { _currentTool = tool; }
bool Canvas::HasCanvas() const { return _hasCanvas; }

void Canvas::Init() {
  _CreateBackground();
}

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
      Vec2f mousePos = Utils::Input::GetMousePosition();
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
  if (!_oBackground) { return; }
  Vec2f monitorSize = Utils::View::GetCurrentMonitorSize().CastTo<float>();
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
  if (_currentTool == nullptr || _oLayers.empty()) { return; }

  UIObject* hitLayer = nullptr;
  for (auto layer : _oLayers) {
    if (layer->CursorAbove()) {
      hitLayer = layer;
      break;
    }
  }

  if (!hitLayer) return;

  // ... calculating hit position
  Vec2f onTextureHitPosition = Utils::Input::GetMousePosition() - hitLayer->position;
  if (Utils::Input::MousePressed()) _currentTool->_HandleMousePressed(hitLayer, onTextureHitPosition);
  else if (Utils::Input::MouseDown()) _currentTool->_HandleMouseDown(hitLayer, onTextureHitPosition);
  else if (Utils::Input::MouseReleased()) _currentTool->_HandleMouseRelease(hitLayer, onTextureHitPosition);

}

void Canvas::AddImage(Image image) {
  // Jeżeli nie ma płótna -> utwórz nową warstwę/płótno w rozmiarze obrazu
  if (!_hasCanvas) {
    UIObject* uiObj = new UIObject();
    uiObj->SetImage(image);
    uiObj->size = {(float)image.width, (float)image.height};
    uiObj->position = Utils::View::GetWindowSize().CastTo<float>() / 2 - uiObj->size / 2; // teraz płótno zaczyna się w (0,0)
    uiObj->zLayer = 0;
    uiObj->color = WHITE;
    uiObj->outlineScale = 0;
    _oLayers.push_back(uiObj);
    _hasCanvas = true;
    return;
  }

  // Jeśli mamy płótno - wklejamy obraz do aktywnej warstwy (tu: warstwa 0 lub _oLayers[active])
  if (_oLayers.empty()) return;
  UIObject* baseLayer = _oLayers[0]; // lub wskaz do aktywnej warstwy jeśli masz "active"

  // Jeśli jest zaznaczenie - wklejamy obraz przycięty do jej rozmiaru w pozycji zaznaczenia
  ImageDraw(&baseLayer->GetImage(), image,
            {0, 0, (float)image.width, (float)image.height},
            {100 - baseLayer->position.x, 100 - baseLayer->position.y, (float)image.width, (float)image.height},
            WHITE);
  baseLayer->UpdateTexture();
}

void Canvas::AddTexture(Texture2D texture) {
  AddImage(LoadImageFromTexture(texture));
}

