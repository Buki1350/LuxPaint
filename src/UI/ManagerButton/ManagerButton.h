#pragma once
#include "../../Render/UIObject.h"
#include "../../Automatition/Updatables/Updatable.h"

class App;

class ManagerButton final : public Updatable {
  UIObject* _uiObject_base = nullptr;
  UIObject* _uiObject_newButton = nullptr;
  UIObject* _uiObject_saveButton = nullptr;
  UIObject* _uiObject_loadButton = nullptr;
  UIObject* _uiObject_settingsButton = nullptr;
  std::vector<UIObject*> _uiObject_buttons;

  float _roundness = 0.2f;
  float _marginScale = 0.01f;
  float _sizeScale = 0.025f;
  float _listElementWidthScale = 0.15f;
  float _listElementHeightScale = 0.03f;
  float _listElementSeparationScale = 0.005f;
  bool _listExpanded = false;

  void _LoadImageFromSystem();
  void _AdjustSizeAndPosition();
  void _CreateBlank(Vec2i size);
public:
  void Init();
  void Update() override;
};