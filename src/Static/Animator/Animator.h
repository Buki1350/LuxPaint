#pragma once
#include "raylib.h"
#include "../../Render/UIObject.h"
#include "../../Defines.h"
#include <vector>

class UIObject; // double declaration

enum MovementType {
  GAUSSIAN,
  LINEAR,
};

class Animator {
  static Vector2 Vector2Lerp(Vector2 start, Vector2 end, float t);
  static Vector2 GetInterpolatedValue(
      Vector2 start, Vector2 end, float t, MovementType movementType);

public:
  struct AnimatedUIObjectsSet {
    UIObject*     uiObjectAddress;
    Vector2       startPosition;
    Vector2       startSize;
    Vector2       targetPosition;
    Vector2       targetSize;
    Vector2       prevTargetPosition;   // new animation check
    Vector2       prevTargetSize;
    float         elapsed    = 0.0f;    //
    float         duration   = 1.0f;    // sekundy
    float         tolerance;
    float         startAlpha   = 1.0f;
    float         targetAlpha  = 1.0f;
    float         prevTargetAlpha = 1.0f;
    MovementType  movementType;
  };


  static std::vector<AnimatedUIObjectsSet> _animatedUIObjects;

  static void Transform(
      UIObject* uiObject,
      Vector2 targetPosition,
      Vector2 targetSize,
      MovementType movementType,
      float duration = 1.0f,
      float tolerance = ANIMATION_DEFAULT_TOLERANCE,
      float targetImageAlpha = 1
  );

  static void SizeUp(UIObject *uiObject,
                     float scale = ANIMATION_DEFAULT_SIZEUP_SCALE,
                     float time = ANIMATION_DEFAULT_SIZEUP_DURATION);
  static void FadeOut(UIObject *uiObject, float time);
  static void FadeIn(UIObject *uiObject, float time);
  static void Reset(UIObject* uiObject, float time = 0.0f);
};