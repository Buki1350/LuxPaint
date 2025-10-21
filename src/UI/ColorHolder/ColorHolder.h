#pragma once
#include "../../Render/UIObject.h"
#include "../../Automatition/Updatables/Updatable.h"
#include "../../Render/Prefabs/Button.h"

class ColorHolder final : public Updatable {
  UIObject* _oBackground = nullptr;
  float _offsetScale = 0.01f;
  float _buttonScale = 0.05f;
  float _separatorScale = 0.01f;

  std::vector<Button*> _buttons;

public:
  void Init();
  void Update() override;
  void _CreateColorPicker();
};
