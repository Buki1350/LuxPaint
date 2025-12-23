#pragma once
#include "../../Automatition/Updatables/Updatable.h"
#include "../../Defines.h"
#include "../../Shared/UIObjects/UIObject.h"
#include "../Singleton/Singleton.h"
#include "AnimationValue.h"
#include "raylib.h"

#include <memory>
#include <vector>

class UIObject;

class Animator final : public Updatable, public Singleton<Animator> {
public:
  std::vector<std::unique_ptr<IAnimationValue>> animatedObjectsValues;

  static void transform(
      UIObject* uiObject,
      Vec2f targetPosition,
      Vec2f targetSize,
      MovementType movementType,
      float duration = 1.0f,
      float tolerance = ANIMATION_TOLERANCE,
      float targetImageAlpha = 1
  );

  void update() override; // instance update

  //... basic - returns current animated value
  static Vec2f animatePosition(UIObject* uiObject, Vec2f targetPosition, float duration, MovementType movementType = GAUSSIAN);
  static Vec2f animateSize(UIObject* uiObject, Vec2f targetSize, float duration, MovementType movementType = GAUSSIAN);
  static Color animateColor(UIObject* uiObject, Color targetColor, float duration, MovementType movementType = GAUSSIAN);
  static float animateImageAlpha(UIObject* uiObject, float targetAlpha, float duration, MovementType movementType = GAUSSIAN);
  static float animateRoundness(UIObject* uiObject, float targetRoundness, float duration, MovementType movementType = GAUSSIAN);
  static float animateOutline(UIObject* uiObject, float targetOutline, float duration, MovementType movementType = GAUSSIAN);
  static void reset(UIObject* uiObject, UIObjectParameter parameter, float time = 0.0f);
  static void reset(UIObject *uiObject, float time = 0.0f);
  static void apply(UIObject* uiObject);

  //... templates
  static void sizeUp(UIObject *uiObject,
                     float scale = ANIMATION_ICON_SIZEUP_SCALE,
                     float time = ANIMATION_SIZEUP_DURATION);
  static void fadeOut(UIObject *uiObject, float time);
  static void fadeIn(UIObject *uiObject, float time);

  //...
  static void terminate(UIObject *uiObject);
  static void terminate(std::vector<UIObject*>& uiObjects);
  static void free(UIObject* uiObject);
  static bool animatorContains(UIObject* uiObject, UIObjectParameter param);

};