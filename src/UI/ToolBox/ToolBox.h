#pragma once
#include "../../PaintTools/ToolSet/ToolSet.h"
#include "../../Automatition/Updatables/Updatable.h"

class ToolBox final : public Updatable {
  UIObject* _oBackground = nullptr;
  UIObject* _activeToolsBackground = nullptr;
  Color activeToolsBackgroundColor {};
  const ToolSet* _currentToolSet = nullptr;
  std::vector<ToolSet> toolSets;

  float uiObjectBorderSeparation = 0.01f;
  float tilesScale = 0.05f;
  float widthScale = 0.04f;
  float marginScale = 0.01f;
  float tilesSeparationScale = 0.01f;


public:
  void Init();
  void Update() override;
  void _ExpandTools(const ToolSet& toolSet);
};
