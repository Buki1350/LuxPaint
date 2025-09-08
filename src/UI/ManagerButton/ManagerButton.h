#pragma once
#include "../../Render/UIObject.h"
#include "../../Updatables/Updatable.h"

class ManagerButton final : public Updatable {
  UIObject* _uiObject_base = nullptr;
  std::vector<UIObject*> _uiObject_buttons;

  float _roundness = 0.2f;
  float _marginScale = 0.01f;
  float _sizeScale = 0.025f;
  float _listElementWidthScale = 0.15f;
  float _listElementHeightScale = 0.03f;
  float _listElementSeparationScale = 0.005f;
  bool _listExpanded = false;

public:
  void Init();
  void Update() override;
  void AdjustSizeAndPosition();
  void HandleClick();
};