#pragma once
#include "../../../Automatition/Updatables/Updatable.h"
#include "../UIObject.h"

#include <functional>

enum ORIENTATION {
  HORIZONTAL = 0,
  VERTICAL = 1
};

class Slider final : public UIObject, public Updatable {
  ORIENTATION _orientation;
  UIObject* _oLine;
  UIObject* _oGrab;
  std::function<void()> _onChangeFunc;
  float _value = 0.0f;
  bool _grabbing = false;

public:
  Slider(ORIENTATION orientation);
  void Update() override;
  void OnValueChanged(std::function<void()> lambdaFunction);
  float GetValue();
  void SetValue(float value);
  void Destroy() override;
};