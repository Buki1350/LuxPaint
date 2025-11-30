#pragma once
#include "../Calculations/Calc.h"
#include "../Utils/Utils.h"

enum MovementType {
  GAUSSIAN,
  LINEAR,
  GAUSSIAN_SHIFTED_FORWARD,
  GAUSSIAN_SHIFTED_BACKWARD
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
class AnimationValue final : public IAnimationValue {
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
    //float overshoot = ANIMATION_OVERSHOOT_STRENGTH;

    switch (movementType) {
    case GAUSSIAN:
      alpha = -cos(t * PI) * 0.5f + 0.5f;
      break;
    case LINEAR:
      break;
    case GAUSSIAN_SHIFTED_FORWARD: {
      float peakTime = 0.75f;
      float t2;
      if (t < peakTime)
        t2 = t / peakTime;
      else
        t2 = (t - peakTime) / (1.0f - peakTime);
      alpha = t < peakTime
              ? -cos(t2 * PI * 0.5f) * 0.5f + 0.5f
              : -cos((t2 * 0.5f + 0.5f) * PI) * 0.5f + 0.5f;
      break;
    }
    case GAUSSIAN_SHIFTED_BACKWARD: {
      float peakTime = 0.25f;
      float t2;
      if (t < peakTime)
        t2 = t / peakTime;
      else
        t2 = (t - peakTime) / (1.0f - peakTime);
      alpha = t < peakTime
              ? -cos(t2 * PI * 0.5f) * 0.5f + 0.5f
              : -cos((t2 * 0.5f + 0.5f) * PI) * 0.5f + 0.5f;
      break;
    }


    }

    alpha = Calc::Clamp(alpha, 0.0f, 1.0f);
    return ValueLerp(startValue, endValue, alpha);
  }

  void Reset(float time) override {
    currentTime = 0.0f;
    duration = time;
    if (!isResetting) {
      std::swap(startValue, targetValue);
      isResetting = true;
    }
  }

  void LocalUpdate() override {
    if (!currentValue) return;

    currentTime += Utils::Time::GetDeltaTime();
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
