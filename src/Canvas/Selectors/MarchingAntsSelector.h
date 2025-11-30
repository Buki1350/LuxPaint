#pragma once
#include "../../Automatition/Initializables/Initializable.h"
#include "../../Automatition/Updatables/Updatable.h"
#include "../../Shared/Singleton/Singleton.h"
#include "../../Shared/UIObjects/UIObject.h"

#include <vector>

struct MarchingSelection {
  UIObject* target;
  UIObject* overlay;
  Texture2D MAImages[8];
};

class MarchingAntsSelector final : public Initializable, public Updatable, public Singleton<MarchingAntsSelector> {
  Texture _GetAntTextureTextureStep(Texture& texture, int step);
public:
  std::vector<MarchingSelection> selections;

  float offset = 0.0f;
  Image gridImage;
  int _frameStep = 0;
  float _timeAccumulator = 0.0f;

  void Init() override;
  void Update() override;
  static void StartOn(UIObject* obj);
  static void StopOn(UIObject *obj);
};
