// #include "AnimationValue.h"
//
// #include "../Utils/Utils.h"
//
// template <typename Value>
// AnimationValue<Value>::AnimationValue(
//     UIObject* uiObject,
//     Value& value,
//     Value targetValue,
//     float duration,
//     UIObjectParameter objectParameter,
//     MovementType movementType
// ) {
//   this->uiObject      = uiObject;
//   this->startValue    = value;
//   this->currentValue  = &value;
//   this->targetValue   = targetValue;
//   this->duration      = duration;
//   this->currentTime   = 0.0f;
//   this->objectParameter = objectParameter;
//   this->movementType  = movementType;
// }
//
//
// template <typename Value>
// Value AnimationValue<Value>::ValueLerp(Value startValue, Value endValue, float t) {
//   return startValue + t * (endValue - startValue);
// }
//
// template <typename Value>
// Value AnimationValue<Value>::ValueFunctionLerp(
//   Value startValue,
//   Value endValue,
//   float t,
//   MovementType movementType
//   ) {
//   float alpha = t;
//   switch (movementType) {
//     case GAUSSIAN:
//       alpha = -cos(t * PI) * .5f + .5f;
//       break;
//     case LINEAR:
//       break;
//     default:
//       break;
//   }
//   return ValueLerp(startValue, endValue, alpha);
// }
//
// template <typename Value>
// void AnimationValue<Value>::Update() {
//   if (!currentValue)
//     return;
//
//   currentTime += Utils::Instance->GetDeltaTime();
//
//   float t = duration > 0.0f ? currentTime / duration : 1.0f;
//   if (t > 1.0f) {
//     t = 1.0f;
//   }
//
//   *currentValue = ValueFunctionLerp(startValue, targetValue, t, movementType);
//
//   if (t >= 1.0f) {
//     *currentValue = targetValue;
//     if (_isResetting) readyToDelete = true;
//   }
// }
//
// template <typename Value>
// void AnimationValue<Value>::Reset(float time) {
//   currentTime = 0.0f;
//   duration = time;
//   std::swap(startValue, targetValue);
//   _isResetting = true;
// }
