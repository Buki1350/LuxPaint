#include "MarchingAntsSelector.h"
#include "../../../Shared/Serializer/Serializer.h"
#include "../../../Shared/UIObjects/UIObjectsManager.h"
#include "Math/Matrx.h"
#include "Shared/Utils/Convert/utiConvert.h"

[[maybe_unused]]
static MarchingAntsSelector instance;

void MarchingAntsSelector::init() {
  gridImage = Serializer::LoadImage("Grids/checker.png");
}

void MarchingAntsSelector::startOn(UIObject* obj) {
  for (auto &sel : instance().selections)
    if (sel.target == obj) return;

  MarchingSelection newSel;
  newSel.target = obj;

  for (int i = 0; i < 8; i++) {
    newSel.MAImages[i] = instance()._getAntTextureTextureStep(obj->getTexture(), i);
  }

  newSel.overlay = new UIObject();
  newSel.overlay->setImage(newSel.MAImages[0]);
  newSel.overlay->size = obj->size;
  newSel.overlay->position = obj->position;
  newSel.overlay->setZLayer(obj->getZLayer() + 1);
  newSel.overlay->imageAlpha = 1.0f;
  newSel.overlay->outlineScale = 0.0f;

  instance().selections.push_back(newSel);
}



void MarchingAntsSelector::update() {
  _timeAccumulator += GetFrameTime();
  if (_timeAccumulator >= 0.1f) { // co 100 ms
    _frameStep = (_frameStep + 1) % 8;
    _timeAccumulator = 0.0f;
  }

  for (auto &sel : selections) {
    sel.overlay->position = sel.target->position;
    sel.overlay->size = sel.target->size;

    // ustawiamy aktualną klatkę
    sel.overlay->setImage(sel.MAImages[_frameStep]);
  }
}



void MarchingAntsSelector::stopOn(UIObject* obj) {
  //if (instance == nullptr) return;

  for (auto it = instance().selections.begin(); it != instance().selections.end(); ++it) {
    if (it->target == obj) {
      it->overlay->isActive = false;

      for (int i = 0; i < 8; i++) {
        UnloadTexture(it->MAImages[i]);
      }

      instance().selections.erase(it);
      break;
    }
  }
}

Texture MarchingAntsSelector::_getAntTextureTextureStep(Texture& texture, int step) {
  Matrx<Color> textureMatrix = uti::convert::textureToMatrix(texture);
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

  Texture2D newTexture = uti::convert::matrixToTexture(textureMatrix);
  return newTexture;
}



