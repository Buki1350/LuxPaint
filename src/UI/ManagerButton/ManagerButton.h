#pragma once
#include "../../Automatition/Updatables/Updatable.h"
#include "../../Shared/UIObjects/UIObject.h"
#include "Automatition/Initializables/Initializable.h"

class Button;
class App;

class ManagerButton final : public UIObject, public Updatable, public Initializable {
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

  void loadImageFromSystem();
  void adjustSizeAndPosition();

public:
  void init() override;
  void update() override;
  void saveImageToSystem();
};