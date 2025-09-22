#pragma once

#include "../../Automatition/Updatables/Updatable.h"
#include "../../Render/UIObject.h"
#include "../Utils/Utils.h"

enum MovementType {
    GAUSSIAN,
    LINEAR,
  };

struct IAnimationValue {
  UIObject*           uiObject = nullptr;
  UIObjectParameter   objectParameter;
  bool                readyToDelete = false;
  virtual ~IAnimationValue() = default;
  virtual void Reset(float time) = 0;
  virtual void LocalUpdate() = 0;
};

template <typename Value>
class AnimationValue : public IAnimationValue {
  public:
  //... linear interpolation variables
  Value               startValue;
  Value*              currentValue;
  Value               targetValue;
  float               currentTime = 0;
  float               duration = 0;
  MovementType        movementType;
  //... flag for animator to handle reset
  bool                isResetting = false;
  //... Animator overrides UIObject, thus if animated value
  //... is needed we take this one
  Value               savedValue;

  AnimationValue(
    UIObject* uiObject,
    Value& value,
    Value targetValue,
    float duration,
    UIObjectParameter objectParameter,
    MovementType movementType = GAUSSIAN
  ) {
    this->uiObject      = uiObject;
    this->startValue    = value;
    this->currentValue  = &value;
    this->targetValue   = targetValue;
    this->duration      = duration;
    this->currentTime   = 0.0f;
    this->objectParameter = objectParameter;
    this->movementType  = movementType;
  }

  Value ValueLerp(Value startValue, Value endValue, float t) {
    return startValue + (endValue - startValue) * t;
  }

  Value ValueFunctionLerp(Value startValue, Value endValue, float t, MovementType movementType) {
    float alpha = t;
    switch (movementType) {
    case GAUSSIAN:
      alpha = -cos(t * PI) * 0.5f + 0.5f;
      break;
    case LINEAR:
      break;
    }
    return ValueLerp(startValue, endValue, alpha);
  }

  void Reset(float time) override {
    currentTime = 0.0f;
    duration = time;
    std::swap(startValue, targetValue);
    isResetting = true;
  }

  void LocalUpdate() override {
    if (!currentValue) return;

    currentTime += Utils::GetDeltaTime();
    float t = duration > 0.0f ? currentTime / duration : 1.0f;
    if (t > 1.0f) t = 1.0f;

    *currentValue = ValueFunctionLerp(startValue, targetValue, t, movementType);
    savedValue = *currentValue;
    if (t >= 1.0f) {
      *currentValue = targetValue;
      if (isResetting) readyToDelete = true;
    }
  }
};

template<>
inline Color AnimationValue<Color>::ValueLerp(Color startValue, Color endValue, float t) {
  return {
    (unsigned char)(startValue.r + t * (endValue.r - startValue.r)),
    (unsigned char)(startValue.g + t * (endValue.g - startValue.g)),
    (unsigned char)(startValue.b + t * (endValue.b - startValue.b)),
    (unsigned char)(startValue.a + t * (endValue.a - startValue.a))
};
}
