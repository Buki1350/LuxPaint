// Animator.cpp (fragment)
#include "Animator.h"
#include <algorithm> // find_if

std::vector<Animator::AnimatedUIObjectsSet> Animator::_animatedUIObjects;

Vector2 Animator::Vector2Lerp(Vector2 start, Vector2 end, float t) {
  return {
    start.x + (end.x - start.x) * t,
    start.y + (end.y - start.y) * t
  };
}

Vector2 Animator::GetInterpolatedValue(Vector2 start, Vector2 end, float t, MovementType movementType) {
  float alpha = t;
  switch (movementType) {
    case GAUSSIAN:
      alpha = -cos(t * PI) * .5f + .5f;
      break;
    case LINEAR:
    default:
      break;
  }
  return Vector2Lerp(start, end, alpha);
}

void Animator::Transform(
  UIObject *uiObject,
  Vector2 targetAbsPos,
  Vector2 targetAbsSize,
  MovementType movementType,
  float duration,
  float tolerance,
  float targetImageAlpha
) {
  auto it = std::find_if(_animatedUIObjects.begin(), _animatedUIObjects.end(),
    [&](const AnimatedUIObjectsSet &anim) { return anim.uiObjectAddress == uiObject; });

  if (it == _animatedUIObjects.end()) {
    AnimatedUIObjectsSet entry;
    entry.uiObjectAddress   = uiObject;
    entry.startPosition     = uiObject->animOffsetPos;
    entry.startSize         = uiObject->animOffsetSize;
    entry.targetPosition    = { targetAbsPos.x - uiObject->position.x, targetAbsPos.y - uiObject->position.y };
    entry.targetSize        = { targetAbsSize.x - uiObject->size.x, targetAbsSize.y - uiObject->size.y };

    entry.startAlpha        = uiObject->imageAlpha;
    entry.targetAlpha       = targetImageAlpha;
    entry.prevTargetAlpha   = targetImageAlpha;

    entry.prevTargetPosition = targetAbsPos;
    entry.prevTargetSize     = targetAbsSize;

    entry.elapsed = 0.0f;
    entry.duration = duration;
    entry.movementType = movementType;
    entry.tolerance = tolerance;

    _animatedUIObjects.push_back(std::move(entry));
    it = std::prev(_animatedUIObjects.end());
  }

  auto &anim = *it;

  bool targetChanged =
      anim.prevTargetPosition.x != targetAbsPos.x || anim.prevTargetPosition.y != targetAbsPos.y ||
      anim.prevTargetSize.x     != targetAbsSize.x  || anim.prevTargetSize.y     != targetAbsSize.y ||
      anim.prevTargetAlpha      != targetImageAlpha;

  if (targetChanged) {
    anim.startPosition = uiObject->animOffsetPos;
    anim.startSize     = uiObject->animOffsetSize;
    anim.targetPosition = { targetAbsPos.x - uiObject->position.x, targetAbsPos.y - uiObject->position.y };
    anim.targetSize     = { targetAbsSize.x - uiObject->size.x, targetAbsSize.y - uiObject->size.y };

    anim.startAlpha     = uiObject->imageAlpha;
    anim.targetAlpha    = targetImageAlpha;

    anim.elapsed = 0.0f;
    anim.duration = duration;
    anim.movementType = movementType;
    anim.tolerance = tolerance;

    anim.prevTargetPosition = targetAbsPos;
    anim.prevTargetSize     = targetAbsSize;
    anim.prevTargetAlpha    = targetImageAlpha;
  }

  anim.elapsed += GetFrameTime();
  float t = anim.duration > 0.0f ? anim.elapsed / anim.duration : 1.0f;
  if (t > 1.0f) t = 1.0f;

  uiObject->animOffsetPos  = GetInterpolatedValue(anim.startPosition, anim.targetPosition, t, anim.movementType);
  uiObject->animOffsetSize = GetInterpolatedValue(anim.startSize, anim.targetSize, t, anim.movementType);

  float alphaT = anim.movementType == GAUSSIAN ? -cos(t * PI) * 0.5f + 0.5f : t;
  uiObject->imageAlpha = anim.startAlpha + (anim.targetAlpha - anim.startAlpha) * alphaT;

  if (t >= 1.0f) {
    uiObject->animOffsetPos  = anim.targetPosition;
    uiObject->animOffsetSize = anim.targetSize;
    uiObject->imageAlpha     = anim.targetAlpha;
  }
}


void Animator::SizeUp(UIObject *uiObject, float scale, float time) {
  // teraz SizeUp tworzy ABSOLUTNY target = aktualna baza + offset powiększenia
  Vector2 offsetSize = { uiObject->size.x * scale, uiObject->size.y * scale };
  Vector2 offsetPos  = { -uiObject->size.x * scale / 2.0f, -uiObject->size.y * scale / 2.0f };

  // przekazujemy ABSOLUTNE wartości (targetAbsPos = basePos + offsetPos, targetAbsSize = baseSize + offsetSize)
  Animator::Transform(uiObject,
      { uiObject->position.x + offsetPos.x, uiObject->position.y + offsetPos.y },
      { uiObject->size.x + offsetSize.x, uiObject->size.y + offsetSize.y },
      GAUSSIAN, time);
}

void Animator::FadeOut(UIObject *uiObject, float time) {
  Animator::Transform(uiObject,
      uiObject->position,
      uiObject->size,
      GAUSSIAN,
      time,
      0.0f,
      0.0f
  );
}

void Animator::FadeIn(UIObject *uiObject, float time) {
  Animator::Transform(uiObject,
      uiObject->position,
      uiObject->size,
      GAUSSIAN,
      time,
      0.0f,
      1.0f
  );
}

void Animator::Reset(UIObject *uiObject, float time) {
  // reset -> celem są ABSOLUTNE bazowe wartości (czyli offsety = 0)
  Animator::Transform(uiObject,
      uiObject->position,
      uiObject->size,
      GAUSSIAN, time);
}