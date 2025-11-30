#pragma once
#include "../../Automatition/Updatables/Updatable.h"
#include "../../Shared/UIObjects/Prefabs/Button.h"
#include "../../Shared/UIObjects/UIObject.h"

class ColorHolder final : public UIObject, public Updatable {
  float _offsetScale = 0.01f;
  float _buttonScale = 0.05f;
  float _separatorScale = 0.01f;

  std::vector<Button*> _buttons;
  int _numberOfSavedColors = 0;

public:
  void Init();
  void Update() override;
  void _CreateColorPicker();

  void SetBackgroundColor(Color color);
};
