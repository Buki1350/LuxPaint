#pragma once
#include "../../PaintTools/ToolSet/ToolSet.h"
#include "../../Automatition/Updatables/Updatable.h"

class ToolBox final : public Updatable {
  UIObject* uiObject = nullptr;
  float uiObjectBorderSeparation = 0.01f;
  float tilesScale = 0.05f;
  float widthScale = 0.04f;
  float marginScale = 0.01f;
  float tilesSeparationScale = 0.01f;

  std::vector<ToolSet> toolSets;

public:
  void Init();
  void Update() override;
};
