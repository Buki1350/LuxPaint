#pragma once
#include "../../Automatition/Updatables/Updatable.h"
#include "../../Shared/UIObjects/Prefabs/Button.h"
#include "../../Shared/UIObjects/UIObject.h"
#include "Automatition/Initializables/Initializable.h"

class ColorHolder final : public UIObject, public Updatable, public Initializable {
  float _offsetScale = 0.01f;
  float _buttonScale = 0.05f;
  float _separatorScale = 0.01f;

  std::vector<Button*> _colorButtons;
  int _numberOfSavedColors = 0;

public:
  void init() override;
  void update() override;
  void _createColorPicker();

  void setBackgroundColor(Color color);
  Color getCurrentColor() const;
};
