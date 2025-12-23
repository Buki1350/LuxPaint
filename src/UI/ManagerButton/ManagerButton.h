#pragma once
#include "../../Automatition/Updatables/Updatable.h"
#include "../../Shared/UIObjects/UIObject.h"

class Button;
class App;

class ManagerButton final : public UIObject, public Updatable {
  Button* _oNewButton = nullptr;
  Button* _oSaveButton = nullptr;
  Button* _oLoadButton = nullptr;
  Button* _oSettingsButton = nullptr;
  std::vector<Button*> _buttons;

  float _marginScale = 0.01f;
  float _sizeScale = 0.025f;
  float _listElementWidthScale = 0.15f;
  float _listElementHeightScale = 0.03f;
  float _listElementSeparationScale = 0.005f;
  bool _listExpanded = false;

  void _loadImageFromSystem();
  void _adjustSizeAndPosition();

public:
  void init();
  void update() override;
  void _saveImageToSystem();
};