#pragma once
#include "../../Automatition/Updatables/Updatable.h"
#include "../../Defines.h"
#include "../../Render/UIObject.h"
#include "../Singleton/Singleton.h"
#include "AnimationValue.h"
#include "raylib.h"

#include <memory>
#include <vector>

class UIObject;

class Animator final : public Updatable, public Singleton<Animator> {
public:
  std::vector<std::unique_ptr<IAnimationValue>> animatedObjectsValues;

  static void Transform(
      UIObject* uiObject,
      Vec2f targetPosition,
      Vec2f targetSize,
      MovementType movementType,
      float duration = 1.0f,
      float tolerance = ANIMATION_DEFAULT_TOLERANCE,
      float targetImageAlpha = 1
  );

  void Update() override; // instance update

  //... basic - returns current animated value
  static Vec2f AnimatePosition(UIObject* uiObject, Vec2f targetPosition, float duration, MovementType movementType);
  static Vec2f AnimateSize(UIObject* uiObject, Vec2f targetSize, float duration, MovementType movementType);
  static Color AnimateColor(UIObject* uiObject, Color targetColor, float duration, MovementType movementType);
  static float AnimateImageAlpha(UIObject* uiObject, float targetAlpha, float duration, MovementType movementType);
  static float AnimateRoundness(UIObject* uiObject, float targetRoundness, float duration, MovementType movementType);
  static void Reset(UIObject* uiObject, float time = 0.0f);

  //... templates
  static void SizeUp(UIObject *uiObject,
                     float scale = ANIMATION_DEFAULT_SIZEUP_SCALE,
                     float time = ANIMATION_DEFAULT_SIZEUP_DURATION);
  static void FadeOut(UIObject *uiObject, float time);
  static void FadeIn(UIObject *uiObject, float time);

  //... other
  static void Free(UIObject* uiObject);
  static bool AnimatorContains(UIObject* uiObject, UIObjectParameter param);

};












// #pragma once
// #include "../../Automatition/Updatables/Updatable.h"
// #include "../../Defines.h"
// #include "../../Render/UIObject.h"
// #include "../Singleton/Singleton.h"
// #include "raylib.h"
//
// #include <vector>
//
// class UIObject; // double declaration
//
// enum MovementType {
//   GAUSSIAN,
//   LINEAR,
// };
//
// class Animator final : public Updatable, public Singleton<Animator> {
//   static Vec2f Vector2Lerp(Vec2f start, Vec2f end, float t);
//   static Vec2f GetInterpolatedValue(
//       Vec2f start, Vec2f end, float t, MovementType movementType);
//
//   struct AnimatedUIObjectsSet {
//     UIObject*     uiObject;
//     Vec2f         startPosition;
//     Vec2f         startSize;
//     Vec2f         targetPosition;
//     Vec2f         targetSize;
//     Vec2f         prevTargetPosition;
//     Vec2f         prevTargetSize;
//     float         elapsed    = 0.0f;
//     float         duration   = 1.0f;
//     float         tolerance;
//     float         startAlpha   = 1.0f;
//     float         targetAlpha  = 1.0f;
//     float         prevTargetAlpha = 1.0f;
//     MovementType  movementType;
//   };
//
// public:
//   static std::vector<AnimatedUIObjectsSet> animatedUIObjects;
//   void Update() override;
//
//   static void Transform(
//       UIObject* uiObject,
//       Vec2f targetPosition,
//       Vec2f targetSize,
//       MovementType movementType,
//       float duration = 1.0f,
//       float tolerance = ANIMATION_DEFAULT_TOLERANCE,
//       float targetImageAlpha = 1
//   );
//
//   static void SizeUp(UIObject *uiObject,
//                      float scale = ANIMATION_DEFAULT_SIZEUP_SCALE,
//                      float time = ANIMATION_DEFAULT_SIZEUP_DURATION);
//   static void FadeOut(UIObject *uiObject, float time);
//   static void FadeIn(UIObject *uiObject, float time);
//   static void Reset(UIObject* uiObject, float time = 0.0f);
// };