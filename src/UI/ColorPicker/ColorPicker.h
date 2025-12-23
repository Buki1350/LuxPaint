#pragma once
#include "../../Shared/UIObjects/Prefabs/InputField.h"
#include "../../Shared/UIObjects/Prefabs/Slider.h"
#include "../../Shared/UIObjects/UIObject.h"
#include "../../Shared/UIObjects/UIObjectsManager.h"
#include "raylib.h"

class ColorPicker final : public UIObject, public Updatable {
  float _marginScale = 0.01f;
  float _spaceUnderRainbowScale = 0.1f;
  float _overallSizeScale = 0.35f;

  Color _backgroundColor{};
  UIObject* _oRainbow;
  InputField* _hexInputField;
  Slider* _sRSlider;
  Slider* _sGSlider;
  Slider* _sBSlider;
  Slider* _sASlider;
  std::vector<Slider*> _sliders;
  UIObject* _oDot;
  std::vector<UIObject*> _sliderLabels; // ... R G B A

  Color _currentColor = WHITE;
  Color __previousColor; // ... on change callback helper

  bool _rainbowClicked = false;

  float _hue = 0.0f;
  float _saturation = 1.0f;
  float _value = 1.0f;
  float _alpha = 1.0f;

  void _generateRainbowTexture();
  void _updateFromSliders();
  void _updateFromHex();
  void _updateFromRainbow();
  void _syncUI();
  std::function<void(Color)> _OnColorChange;
  std::function<void()> _OnDestroy;

public:
  ColorPicker();
  void update() override;
  void destroy() override;
  Color getColor() const;
  void setColor(Color color);
  void onColorChange(std::function<void(Color)> callback);
  void onDestroy(std::function<void()> labdaFunction);

};
