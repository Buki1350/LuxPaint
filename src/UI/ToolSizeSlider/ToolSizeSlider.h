#pragma once
#include "../../Automatition/Updatables/Updatable.h"
#include "../../Shared/UIObjects/Prefabs/InputField.h"
#include "../../Shared/UIObjects/Prefabs/Slider.h"
#include "../../Shared/UIObjects/UIObject.h"

class ToolSizeSlider final : public UIObject, public Updatable {
  float _widthScale = 0.1f;
  float _heightScale = 0.25f;
  Slider* _slider = nullptr;
  float _maxSliderValue = 50.0f;
  InputField* _inputField = nullptr;

  Vec2f _CalculateHiddenPosition();
  Vec2f _CalculateShownPosition();
  void _Show();
  void _Hide();
  float _GetCurrentValue();

public:
  void Init();
  void Update() override;
};