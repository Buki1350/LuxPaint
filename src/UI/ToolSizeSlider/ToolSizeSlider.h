#pragma once
#include "../../Automatition/Updatables/Updatable.h"
#include "../../Shared/UIObjects/Prefabs/InputField.h"
#include "../../Shared/UIObjects/Prefabs/Slider.h"
#include "../../Shared/UIObjects/UIObject.h"

class Tool;
class ToolSizeSlider final : public UIObject, public Updatable {
  float _widthScale = 0.1f;
  float _heightScale = 0.25f;
  Slider* _slider = nullptr;
  float _maxSliderValue = 50.0f;
  InputField* _inputField = nullptr;
  int minVal = -1, maxVal = -1;
  Tool* _currentTool = nullptr;

  Vec2f _calculateHiddenPosition();
  Vec2f _calculateShownPosition();
  void _updateSliderFromInput();
  void _updateInputFromSlider(float sliderValue);
  void _show();
  void _hide();
  float _getCurrentValue();

public:
  void init();
  void update() override;
};