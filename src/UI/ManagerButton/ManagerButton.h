#pragma once
#include "../../Automatition/Updatables/Updatable.h"
#include "../../Render/UIObject.h"

class Button;
class App;

class ManagerButton final : public Updatable {
  UIObject* _oBackground = nullptr;
  Button* _oNewButton = nullptr;
  Button* _oSaveButton = nullptr;
  Button* _oLoadButton = nullptr;
  Button* _oSettingsButton = nullptr;
  std::vector<Button*> _buttons;

  float _roundness = 0.2f;
  float _marginScale = 0.01f;
  float _sizeScale = 0.025f;
  float _listElementWidthScale = 0.15f;
  float _listElementHeightScale = 0.03f;
  float _listElementSeparationScale = 0.005f;
  bool _listExpanded = false;

  void _LoadImageFromSystem();
  void _AdjustSizeAndPosition();

public:
  void Init();
  void Update() override;
  void SetBackgroundColor(Color color);
};