#pragma once
#include "../../../Automatition/Updatables/Updatable.h"
#include "../UIObject.h"

#include <functional>

enum ORIENTATION {
  HORIZONTAL = 0,
  VERTICAL = 1
};

class  Slider final : public UIObject, public Updatable {
  ORIENTATION _orientation;
  UIObject* _oLine;
  UIObject* _oGrab;
  std::function<void()> _onChangeFunc;
  std::function<void()> _onReleaseFunc;
  float _value = 0.0f;
  bool _grabbing = false;

public:
  Slider(ORIENTATION orientation);
  void update() override;
  void onValueChanged(std::function<void()> lambdaFunction);
  float getValue();
  void setValue(float value);
  void onRelease(std::function<void()> lambdaFunction);
  void destroy() override;
};