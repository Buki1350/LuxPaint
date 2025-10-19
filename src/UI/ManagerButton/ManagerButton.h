#pragma once
#include "../../Automatition/Updatables/Updatable.h"
#include "../../Render/UIObject.h"

class Button;
class App;

class ManagerButton final : public Updatable {
  UIObject* _uiObject_base = nullptr;
  Button* _uiObject_newButton = nullptr;
  Button* _uiObject_saveButton = nullptr;
  Button* _uiObject_loadButton = nullptr;
  Button* _uiObject_settingsButton = nullptr;
  std::vector<Button*> _uiObject_buttons;

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