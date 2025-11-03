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
void Canvas::SetSelection(const Rectangle& rect) {
  // jeśli poprzedni target był, usuń marching ants i zwolnij
  if (_selectionTarget) {
    MarchingAntsSelector::Instance->StopOn(_selectionTarget);
    _selectionTarget->Destroy();
    _selectionTarget = nullptr;
  }

  _selection = rect;

  if (_selection.width <= 0 || _selection.height <= 0) {
    return; // puste zaznaczenie
  }

  // Utwórz tymczasowy UIObject jako "target" dla MarchingAntsSelector.
  // Ten obiekt nie musi mieć prawdziwej tekstury zawartości, wystarczy tekstura o właściwym rozmiarze.
  _selectionTarget = new UIObject();
  // Tworzymy pustą image/texture o rozmiarze selekcji (1:1 w pikselach ekranu).
  // MarchingAntsSelector używa target->GetTexture() tylko po to by odczytać rozmiar,
  // ale w Twojej implementacji korzysta z Utils::TextureToMatrix(texture) i jego rozmiaru.
  // Dlatego tworzymy małą teksturę o rozmiarze _selection.width/_selection.height.
  int w = (int)ceilf(_selection.width);
  int h = (int)ceilf(_selection.height);
  Image tmp = GenImageColor(w, h, BLANK); // pusty obraz
  _selectionTarget->SetImage(tmp);
  UnloadImage(tmp); // mamy już teksturę w UIObject (kopię) - jeśli SetImage kopiował

  _selectionTarget->position = { _selection.x, _selection.y };
  _selectionTarget->size = { _selection.width, _selection.height };
  _selectionTarget->zLayer = 1000; // na wierzchu

  // Uruchamiamy marching ants na tym targetcie (to utworzy overlay z animacją)
  MarchingAntsSelector::Instance->StartOn(_selectionTarget);
}

void Canvas::ClearSelection() {
  if (_selectionTarget) {
    MarchingAntsSelector::Instance->StopOn(_selectionTarget);
    _selectionTarget->Destroy();
    _selectionTarget = nullptr;
  }
  _selection = {0,0,0,0};
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
  _currentTool->HandleClick(hitLayer, onTextureHitPosition);

}

void Canvas::AddImage(Image image) {
  // Jeżeli nie ma płótna -> utwórz nową warstwę/płótno w rozmiarze obrazu
  if (!_hasCanvas) {
    UIObject* uiObj = new UIObject();
    uiObj->SetImage(image);
    uiObj->size = {(float)image.width, (float)image.height};
    uiObj->position = {0, 0}; // teraz płótno zaczyna się w (0,0)
    uiObj->zLayer = 0;
    uiObj->color = WHITE;
    _oLayers.push_back(uiObj);
    _hasCanvas = true;
    return;
  }

  // Jeśli mamy płótno - wklejamy obraz do aktywnej warstwy (tu: warstwa 0 lub _oLayers[active])
  if (_oLayers.empty()) return;
  UIObject* baseLayer = _oLayers[0]; // lub wskaz do aktywnej warstwy jeśli masz "active"

  // Jeśli jest zaznaczenie - wklejamy obraz przycięty do jej rozmiaru w pozycji zaznaczenia
  Rectangle sel = GetSelection();
  if (sel.width <= 0 || sel.height <= 0) {
    // brak zaznaczenia -> możemy wklejać w (100,100) lub nic nie robić; tu wklejamy w lewy-górny róg
    ImageDraw(&baseLayer->GetImage(), image,
              {0, 0, (float)image.width, (float)image.height},
              {100 - baseLayer->position.x, 100 - baseLayer->position.y, (float)image.width, (float)image.height},
              WHITE);
    baseLayer->UpdateTexture();
    return;
  }

  // Przytnij źródło i docelowe prostokąty do obszaru zaznaczenia i rozmiaru obrazu
  // dst w lokalnych współrzędnych obrazu warstwy
  float dstX = sel.x - baseLayer->position.x;
  float dstY = sel.y - baseLayer->position.y;
  int dstW = (int)sel.width;
  int dstH = (int)sel.height;

  // Wymiary źródła (części obrazu którą wklejamy) - przytnij do rozmiaru image
  int srcW = fmin(image.width, dstW);
  int srcH = fmin(image.height, dstH);

  Rectangle srcRec = {0, 0, (float)srcW, (float)srcH};
  Rectangle dstRec = {dstX, dstY, (float)srcW, (float)srcH};

  ImageDraw(&baseLayer->GetImage(), image, srcRec, dstRec, WHITE);
  baseLayer->UpdateTexture();

  // po wklejeniu możemy (opcjonalnie) wyczyścić zaznaczenie
  ClearSelection();
}



void Canvas::AddTexture(Texture2D texture) {
  AddImage(LoadImageFromTexture(texture));
}

