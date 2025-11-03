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
      float tolerance = ANIMATION_TOLERANCE,
      float targetImageAlpha = 1
  );

  void Update() override; // instance update

  //... basic - returns current animated value
  static Vec2f AnimatePosition(UIObject* uiObject, Vec2f targetPosition, float duration, MovementType movementType = GAUSSIAN);
  static Vec2f AnimateSize(UIObject* uiObject, Vec2f targetSize, float duration, MovementType movementType = GAUSSIAN);
  static Color AnimateColor(UIObject* uiObject, Color targetColor, float duration, MovementType movementType = GAUSSIAN);
  static float AnimateImageAlpha(UIObject* uiObject, float targetAlpha, float duration, MovementType movementType = GAUSSIAN);
  static float AnimateRoundness(UIObject* uiObject, float targetRoundness, float duration, MovementType movementType = GAUSSIAN);
  static float AnimateOutline(UIObject* uiObject, float targetOutline, float duration, MovementType movementType = GAUSSIAN);
  static void Reset(UIObject* uiObject, float time = 0.0f);
  static void Apply(UIObject* uiObject);

  //... templates
  static void SizeUp(UIObject *uiObject,
                     float scale = ANIMATION_ICON_SIZEUP_SCALE,
                     float time = ANIMATION_SIZEUP_DURATION);
  static void FadeOut(UIObject *uiObject, float time);
  static void FadeIn(UIObject *uiObject, float time);

  //...
  static void Terminate(UIObject *uiObject);
  static void Terminate(std::vector<UIObject*>& uiObjects);
  static void Free(UIObject* uiObject);
  static bool AnimatorContains(UIObject* uiObject, UIObjectParameter param);

};