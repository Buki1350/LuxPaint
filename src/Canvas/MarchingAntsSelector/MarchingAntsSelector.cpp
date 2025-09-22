#include "MarchingAntsSelector.h"
#include "../../Render/UIObjectManager.h"
#include "../../StaticShared/FilesManager/FilesManager.h"
#include "../../StaticShared/Utils/Utils.h"

static MarchingAntsSelector _;

void MarchingAntsSelector::Init() {
  gridImage = FilesManager::LoadImage("Grids/checker.png");
}

void MarchingAntsSelector::StartOn(UIObject* obj) {
  for (auto &sel : selections)
    if (sel.target == obj) return;

  MarchingSelection newSel;
  newSel.target = obj;

  for (int i = 0; i < 8; i++) {
    newSel.MAImages[i] = GetAntTextureTextureStep(obj->GetTexture(), i);
  }

  newSel.overlay = UIObjectManager::CreateUIObject();
  newSel.overlay->SetImage(newSel.MAImages[0]);
  newSel.overlay->size = obj->size * 0.01f;
  newSel.overlay->zLayer = obj->zLayer;
  newSel.overlay->imageAlpha = 1.0f;
  newSel.overlay->outlineScale = 0.0f;

  selections.push_back(newSel);
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
  for (auto it = selections.begin(); it != selections.end(); ++it) {
    if (it->target == obj) {
      it->overlay->isActive = false;

      // zwalniamy klatki z GPU
      for (int i = 0; i < 8; i++) {
        UnloadTexture(it->MAImages[i]);
      }

      selections.erase(it);
      break;
    }
  }
}

Texture MarchingAntsSelector::GetAntTextureTextureStep(Texture& texture, int step) {
  Matx<Color> textureMatrix = Utils::TextureToMatrix(texture);
  Vec2i size = textureMatrix.size();

  int borderThickness = 2;
  int patternLength   = 64;   // "długość" całego wzoru
  int totalFrames     = 8;    // liczba klatek animacji
  Color c1 = BLACK;
  Color c2 = WHITE;

  // przeskalowanie kroku do długości wzoru
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

  Texture2D newTexture = Utils::MatrixToTexture(textureMatrix);
  return newTexture;
}



