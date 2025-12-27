#pragma once
#include "../../Automatition/Updatables/Updatable.h"
#include "../../Shared/UIObjects/Prefabs/InputField.h"
#include "../../Shared/UIObjects/Prefabs/Slider.h"
#include "../../Shared/UIObjects/UIObject.h"

#include <memory>

class Tool;
class ToolSizeSlider final : public UIObject, public Updatable {
  float _widthScale = 0.1f;
  float _heightScale = 0.25f;
  std::unique_ptr<Slider> _slider = nullptr;
  float _maxSliderValue = 50.0f;
  InputField* _inputField = nullptr;
  int _minVal = -1, _maxVal = -1, _floatValue = 1;
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