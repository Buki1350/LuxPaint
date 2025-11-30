#include "MarchingAntsSelector.h"
#include "../../Shared/FilesManager/FilesManager.h"
#include "../../Shared/Utils/Utils.h"
#include "../../Shared/UIObjects/UIObjectsManager.h"

[[maybe_unused]]
static MarchingAntsSelector instance;

void MarchingAntsSelector::Init() {
  gridImage = FilesManager::LoadImage("Grids/checker.png");
}

void MarchingAntsSelector::StartOn(UIObject* obj) {
  for (auto &sel : Instance->selections)
    if (sel.target == obj) return;

  MarchingSelection newSel;
  newSel.target = obj;

  for (int i = 0; i < 8; i++) {
    newSel.MAImages[i] = Instance->_GetAntTextureTextureStep(obj->GetTexture(), i);
  }

  newSel.overlay = new UIObject();
  newSel.overlay->SetImage(newSel.MAImages[0]);
  newSel.overlay->size = obj->size;
  newSel.overlay->position = obj->position;
  newSel.overlay->zLayer = obj->zLayer + 1;
  newSel.overlay->imageAlpha = 1.0f;
  newSel.overlay->outlineScale = 0.0f;

  Instance->selections.push_back(newSel);
}



void MarchingAntsSelector::Update() {
  _timeAccumulator += GetFrameTime();
  if (_timeAccumulator >= 0.1f) { // co 100 ms
    _frameStep = (_frameStep + 1) % 8;
    _timeAccumulator = 0.0f;
  }

  for (auto &sel : selections) {
    sel.overlay->position = sel.target->position;
    sel.overlay->size = sel.target->size;

    // ustawiamy aktualną klatkę
    sel.overlay->SetImage(sel.MAImages[_frameStep]);
  }
}



void MarchingAntsSelector::StopOn(UIObject* obj) {
  if (Instance == nullptr) return;

  for (auto it = Instance->selections.begin(); it != Instance->selections.end(); ++it) {
    if (it->target == obj) {
      it->overlay->isActive = false;

      for (int i = 0; i < 8; i++) {
        UnloadTexture(it->MAImages[i]);
      }

      Instance->selections.erase(it);
      break;
    }
  }
}

Texture MarchingAntsSelector::_GetAntTextureTextureStep(Texture& texture, int step) {
  Matrx<Color> textureMatrix = Utils::Convert::TextureToMatrix(texture);
  Vec2i size = textureMatrix.size();

  int borderThickness = 2;
  int patternLength   = 16;   // "długość" całego wzoru
  int totalFrames     = 8;    // liczba klatek animacji
  Color c1 = BLACK;
  Color c2 = BLANK;

  int phase = (step * patternLength) / totalFrames;

  for (int y = 0; y < size.y; y++) {
    for (int x = 0; x < size.x; x++) {
      bool onBorder =
          (x < borderThickness) ||
          (x >= size.x - borderThickness) ||
          (y < borderThickness) ||
          (y >= size.y - borderThickness);

      if (!onBorder) {
        textureMatrix[y][x] = {0,0,0,0};
      } else {
        int index = (x + y + phase) % patternLength;
        textureMatrix[y][x] = (index < patternLength/2) ? c1 : c2;
      }
    }
  }

  Texture2D newTexture = Utils::Convert::MatrixToTexture(textureMatrix);
  return newTexture;
}



