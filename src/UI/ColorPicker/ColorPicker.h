#pragma once
#include "../../Render/Prefabs/InputField.h"
#include "../../Render/Prefabs/Slider.h"
#include "../../Render/UIObject.h"
#include "../../Render/UIObjectsManager.h"
#include "../../StaticShared/Utils/Utils.h"
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

  void _GenerateRainbowTexture();
  void _UpdateFromSliders();
  void _UpdateFromHex();
  void _UpdateFromRainbow();
  void _SyncUI();
  std::function<void(Color)> _OnColorChange;
  std::function<void()> _OnDestroy;

public:
  ColorPicker();
  void Update() override;
  void Destroy() override;
  Color GetColor() const;
  void SetColor(Color color);
  void OnColorChange(std::function<void(Color)> callback);
  void OnDestroy(std::function<void()> labdaFunction);

};
