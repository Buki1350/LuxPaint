#include "Canvas.h"

#include "../../App/App.h"
#include "../../Math/Vec2.h"
#include "../../Shared/DelayedAction/DelayedAction_until.h"
#include "../../Shared/Serializer/Serializer.h"
#include "Selectors/MarchingAntsSelector.h"
#include "Shared/SnapshotsManager/SnapshotsTypes/ImageSnapshot.h"
#include "Shared/Utils/View/utiView.h"

#include <iostream>

void Canvas::init() {
  _createBackground();
}

void Canvas::update() {
  _handleZoomAndDrag();
  _updateBackground();
  _handlePainting();
  //_handleCropping(); // ... TODO: bugged
}

bool Canvas::_canDrag() {
  return IsKeyDown(KEY_SPACE) || IsMouseButtonDown(MOUSE_MIDDLE_BUTTON);
}

void Canvas::_handleZoomAndDrag() {
  Vec2f mouse = GetMousePosition();

  float wheel = GetMouseWheelMove();
  if (wheel != 0) {
    for (auto oImg : _oLayers) {
      Vec2f mousePos = uti::input::getMousePosition();
      Vec2f cursorOffset = mousePos - oImg->position;

      Vec2f newSize = oImg->size * (1.0f + wheel * 0.1f);
      Vec2f scale = Vec2f{newSize.x / oImg->size.x, newSize.y / oImg->size.y};

      oImg->position += Vec2f{cursorOffset.x * (1.0f - scale.x),
                              cursorOffset.y * (1.0f - scale.y)};

      oImg->size = newSize;
    }
  }

  if (_canDrag()) {
    SetMouseCursor(MOUSE_CURSOR_RESIZE_ALL);

    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
      for (auto it = _oLayers.rbegin(); it != _oLayers.rend(); ++it) {
        UIObject *img = *it;
        if (img->cursorAbove()) {
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

void Canvas::_createBackground() {
  this->color = BLANK;
  this->imageAlpha = 0;
  this->outlineScale = 0;
}

void Canvas::_updateBackground() {
  Vec2f monitorSize = uti::view::getCurrentMonitorSize().CastTo<float>();
  this->size = {(float)monitorSize.x, (float)monitorSize.y};
  this->position = {0, 0};
}

void Canvas::_handleOutline() {
  if (_oActiveImage) {
    MarchingAntsSelector::instance().startOn(_oActiveImage);
  }
  else {
    // Możesz tu ewentualnie usuwać wszystkie, jak chcesz
    // lub wywoływać StopOn() dla konkretnego
  }
}

void Canvas::_handlePainting() {
  if (_currentTool == nullptr || _oLayers.empty() || _isCropping) {
    return;
  }

  UIObject* hitLayer = nullptr;
  for (auto layer : _oLayers) {
    if (layer->cursorAbove())
      hitLayer = layer; // ... last layer -> the one on top
  }

  if (!hitLayer) return;
  // ... 0 division prevention
  if (hitLayer->size.x <= 0.0001f || hitLayer->size.y <= 0.0001f) return;

  if (uti::input::mousePressed())
    _currentTool->handleMousePressed(hitLayer);
  else if (uti::input::mouseDown())
    _currentTool->handleMouseDown(hitLayer);
  else if (uti::input::mouseReleased()) {
    _currentTool->handleMouseRelease(hitLayer);

  }
}

void Canvas::_handleCropping() {
  if (_oLayers.empty()) return;

  UIObject* firstLayer = _oLayers[0];
  UIObject* lastLayer = _oLayers[_oLayers.size() - 1];

  Vec2f mousePosition = uti::input::getMousePosition();
  UIObjEdge edgeUnderCursor = _getHoveredEdge(firstLayer, mousePosition);

  if (!_isCropping) {
      switch (edgeUnderCursor) {
          case UIObjEdge::LEFT:
          case UIObjEdge::RIGHT:
              SetMouseCursor(MOUSE_CURSOR_RESIZE_EW);
              break;
          case UIObjEdge::TOP:
          case UIObjEdge::BOTTOM:
              SetMouseCursor(MOUSE_CURSOR_RESIZE_NS);
              break;
          default:
              SetMouseCursor(MOUSE_CURSOR_DEFAULT);
              break;
      }
  }

  if (uti::input::mousePressed() && edgeUnderCursor != UIObjEdge::NONE) {
    _isCropping = true;
    _activeEdge = edgeUnderCursor;
    _oResizePreview = new UIObject();
    _oResizePreview->color = GRAY;
    _oResizePreview->color.a = 255/2; // ... for semi transparency
    _oResizePreview->position = firstLayer->position;
    _oResizePreview->size = firstLayer->size;
    _oResizePreview->setZLayer(lastLayer->getZLayer() + 1);
  }

  // ... new size calculating
  if (_isCropping && uti::input::mouseDown()) {
    switch (_activeEdge) {
      case UIObjEdge::LEFT: {
        _oResizePreview->position.x = mousePosition.x;
        float deltaX = firstLayer->position.x - _oResizePreview->position.x;
        _oResizePreview->size.x = firstLayer->size.x + deltaX;

        if (_oResizePreview->size.x < MIN_CANVAS_SIZE)
          _oResizePreview->size.x = MIN_CANVAS_SIZE;
        break;
      }
      case UIObjEdge::RIGHT: {
          float newWidth = mousePosition.x - firstLayer->position.x;
          if (newWidth < MIN_CANVAS_SIZE) newWidth = MIN_CANVAS_SIZE;
          _oResizePreview->size.x = newWidth;
          break;
      }
      case UIObjEdge::TOP: {
        _oResizePreview->position.y = mousePosition.y;
        float deltaY = firstLayer->position.y - _oResizePreview->position.y;
        _oResizePreview->size.y = firstLayer->size.y + deltaY;

        if (_oResizePreview->size.y < MIN_CANVAS_SIZE)
          _oResizePreview->size.y = MIN_CANVAS_SIZE;
        break;
      }
      case UIObjEdge::BOTTOM: {
          float newHeight = mousePosition.y - firstLayer->position.y;
          if (newHeight < MIN_CANVAS_SIZE) newHeight = MIN_CANVAS_SIZE;
          _oResizePreview->size.y = newHeight;
          break;
      }
      default:
          break;
    }
  }

  // ... bitmap change
  if (_isCropping && uti::input::mouseReleased()) {
    _isCropping = false;

    Image src = firstLayer->getImage();
    if (src.data != nullptr && src.width > 0 && src.height > 0) {

      Vec2f texSize = {(float)src.width, (float)src.height};
      Vec2f dispSize = firstLayer->size;

      Vec2f scale = texSize / dispSize;

      float cropX = (_oResizePreview->position.x - firstLayer->position.x) * scale.x;
      float cropY = (_oResizePreview->position.y - firstLayer->position.y) * scale.y;
      float cropW = _oResizePreview->size.x * scale.x;
      float cropH = _oResizePreview->size.y * scale.y;

      Rectangle cropRect = {
        floorf(cropX),
        floorf(cropY),
        floorf(cropW),
        floorf(cropH)
      };

      if (cropRect.x < 0) cropRect.x = 0;
      if (cropRect.y < 0) cropRect.y = 0;
      if (cropRect.x + cropRect.width  > src.width)  cropRect.width  = src.width  - cropRect.x;
      if (cropRect.y + cropRect.height > src.height) cropRect.height = src.height - cropRect.y;

      Image newImg = GenImageColor(cropRect.width, cropRect.height, BLANK);

      int offsetX = (int)-cropRect.x;
      int offsetY = (int)-cropRect.y;

      Rectangle srcRect = { 0, 0, (float)src.width, (float)src.height };
      Rectangle dstRect = { (float)offsetX, (float)offsetY, (float)src.width, (float)src.height };
      ImageDraw(&newImg, src, srcRect, dstRect, WHITE);
      firstLayer->setImage(newImg);
      UnloadImage(newImg);

    }

    firstLayer->size = _oResizePreview->size;
    firstLayer->position = _oResizePreview->position;

    _oResizePreview->destroy();
    _activeEdge = UIObjEdge::NONE;
    SetMouseCursor(MOUSE_CURSOR_DEFAULT);
  }
}

Canvas::UIObjEdge Canvas::_getHoveredEdge(UIObject* layer, Vec2f mp) {
  float x1 = layer->position.x;
  float y1 = layer->position.y;
  float x2 = x1 + layer->size.x;
  float y2 = y1 + layer->size.y;

  if (fabs(mp.x - x1) < cropGrabTolerance) return UIObjEdge::LEFT;
  if (fabs(mp.x - x2) < cropGrabTolerance) return UIObjEdge::RIGHT;
  if (fabs(mp.y - y1) < cropGrabTolerance) return UIObjEdge::TOP;
  if (fabs(mp.y - y2) < cropGrabTolerance) return UIObjEdge::BOTTOM;
  return UIObjEdge::NONE;
}

void Canvas::addImage(Image image) {
  if (_oLayers.size() == 0) {
    UIObject* uiObj = new UIObject();
    uiObj->setImage(image);
    uiObj->size = {(float)image.width, (float)image.height};
    uiObj->position = uti::view::getWindowSize().CastTo<float>() / 2 - uiObj->size / 2;
    uiObj->setZLayer(2);
    uiObj->color = WHITE;
    uiObj->outlineScale = 0;
    uiObj->name = "newCanvas";
    _oLayers.push_back(uiObj);
    return;
  }

  // Jeśli mamy płótno - wklejamy obraz do aktywnej warstwy (tu: warstwa 0 lub _oLayers[active])
  if (_oLayers.empty()) return;
  UIObject* baseLayer = _oLayers[0]; // lub wskaz do aktywnej warstwy jeśli masz "active"

  // Jeśli jest zaznaczenie - wklejamy obraz przycięty do jej rozmiaru w pozycji zaznaczenia
  // ImageDraw(&baseLayer->GetImage(), image,
  //           {0, 0, (float)image.width, (float)image.height},
  //           {100 - baseLayer->position.x, 100 - baseLayer->position.y, (float)image.width, (float)image.height},
  //           WHITE);
  // baseLayer->UpdateTexture();
}

Image Canvas::exportAsImage() {
  if (_oLayers.empty()) {
    return GenImageColor(1, 1, BLANK);
  }

  UIObject* first = _oLayers[0];
  Image result = ImageCopy(first->getImage());

  for (int i = 1; i < _oLayers.size(); i++) {
    UIObject* layer = _oLayers[i];
    Image layerImg = layer->getImage();

    if (layerImg.data == nullptr)
      continue;

    Vec2f pos = layer->position - first->position;

    Rectangle src = {0, 0, (float)layerImg.width, (float)layerImg.height};
    Rectangle dst = {pos.x, pos.y, (float)layerImg.width, (float)layerImg.height};

    ImageDraw(&result, layerImg, src, dst, WHITE);
  }

  return result;
}

void Canvas::addTexture(Texture2D texture) {
  addImage(LoadImageFromTexture(texture));
}

const std::vector<UIObject *>& Canvas::getLayersInfo() {
  return _oLayers;
}

void Canvas::setCurrentColor(Color color) { _currentColor = color; }

Color Canvas::getCurrentColor() { return _currentColor; }

void Canvas::setCurrentTool(Tool *tool) { _currentTool = tool; }
const Tool *Canvas::getCurrentToolRO() { return _currentTool; }
Tool *Canvas::getCurrentTool() { return _currentTool; }



