#include "Animator.h"
#include "AnimationValue.h"
#include <algorithm> // find_if

static Animator _;

void Animator::Transform(
  UIObject *uiObject, Vec2f targetAbsPos, Vec2f targetAbsSize,
  MovementType movementType, float duration, float tolerance, float targetImageAlpha
  ) {
  AnimatePosition(uiObject, targetAbsPos, duration, movementType);
  AnimateSize(uiObject, targetAbsSize, duration, movementType);
  AnimateImageAlpha(uiObject, targetImageAlpha, duration, movementType);
}

void Animator::Update() {
  animatedObjectsValues.erase(
      std::remove_if(animatedObjectsValues.begin(), animatedObjectsValues.end(),
                     [](const std::unique_ptr<IAnimationValue>& anim){ return anim->readyToDelete; }),
      animatedObjectsValues.end()
  );

  // update wszystkich
  for (auto& anim : animatedObjectsValues) {
    anim->LocalUpdate();
  }
}

Vec2f Animator::AnimatePosition(UIObject *uiObject, Vec2f targetPosition, float duration, MovementType movementType) {
    for (auto& anim : Instance->animatedObjectsValues) {
        if (anim->uiObject == uiObject && anim->objectParameter == POSITION) {
            auto av = static_cast<AnimationValue<Vec2f>*>(anim.get());

            if (av->targetValue == targetPosition)
                return av->savedValue; // nic nowego, zwróć bieżącą wartość

            // restart animacji od bieżącego stanu
            av->startValue   = av->savedValue;
            av->currentTime  = 0.0f;
            av->duration     = duration;
            av->targetValue  = targetPosition;
            av->movementType = movementType;

            return av->savedValue;
        }
    }

    // nie znaleziono istniejącej animacji → nowa
    auto newAV = std::make_unique<AnimationValue<Vec2f>>(
        uiObject, uiObject->position, targetPosition, duration, POSITION, movementType
    );
    Vec2f start = newAV->startValue;
    Instance->animatedObjectsValues.push_back(std::move(newAV));
    return start;
}

Vec2f Animator::AnimateSize(UIObject *uiObject, Vec2f targetSize, float duration, MovementType movementType) {
    for (auto& anim : Instance->animatedObjectsValues) {
        if (anim->uiObject == uiObject && anim->objectParameter == SIZE) {
            auto av = static_cast<AnimationValue<Vec2f>*>(anim.get());

            if (av->targetValue == targetSize)
                return av->savedValue;

            av->startValue   = av->savedValue;
            av->currentTime  = 0.0f;
            av->duration     = duration;
            av->targetValue  = targetSize;
            av->movementType = movementType;

            return av->savedValue;
        }
    }

    auto newAV = std::make_unique<AnimationValue<Vec2f>>(
        uiObject, uiObject->size, targetSize, duration, SIZE, movementType
    );
    Vec2f start = newAV->startValue;
    Instance->animatedObjectsValues.push_back(std::move(newAV));
    return start;
}

Color Animator::AnimateColor(UIObject *uiObject, Color targetColor, float duration, MovementType movementType) {
    for (auto& anim : Instance->animatedObjectsValues) {
        if (anim->uiObject == uiObject && anim->objectParameter == COLOR) {
            auto av = static_cast<AnimationValue<Color>*>(anim.get());

            if (av->targetValue.r == targetColor.r &&
                av->targetValue.g == targetColor.g &&
                av->targetValue.b == targetColor.b &&
                av->targetValue.a == targetColor.a)
                return av->savedValue;

            av->startValue   = av->savedValue;
            av->currentTime  = 0.0f;
            av->duration     = duration;
            av->targetValue  = targetColor;
            av->movementType = movementType;

            return av->savedValue;
        }
    }

    auto newAV = std::make_unique<AnimationValue<Color>>(
        uiObject, uiObject->color, targetColor, duration, COLOR, movementType
    );
    Color start = newAV->startValue;
    Instance->animatedObjectsValues.push_back(std::move(newAV));
    return start;
}

float Animator::AnimateImageAlpha(UIObject *uiObject, float targetAlpha,
                                  float duration, MovementType movementType) {
  for (auto &anim : Instance->animatedObjectsValues) {
    if (anim->uiObject == uiObject && anim->objectParameter == IMAGE_ALPHA) {
      auto av = static_cast<AnimationValue<float> *>(anim.get());

      if (av->targetValue == targetAlpha)
        return av->savedValue;

      av->startValue = av->savedValue;
      av->currentTime = 0.0f;
      av->duration = duration;
      av->targetValue = targetAlpha;
      av->movementType = movementType;

      return av->savedValue;
    }
  }

  auto newAV = std::make_unique<AnimationValue<float>>(
      uiObject, uiObject->imageAlpha, targetAlpha, duration, IMAGE_ALPHA,
      movementType);
  float start = newAV->startValue;
  Instance->animatedObjectsValues.push_back(std::move(newAV));
  return start;
}

float Animator::AnimateRoundness(UIObject *uiObject, float targetRoundness,
                                 float duration, MovementType movementType) {
  for (auto& anim : Instance->animatedObjectsValues) {
    if (anim->uiObject == uiObject && anim->objectParameter == ROUNDNESS) {
      auto av = static_cast<AnimationValue<float>*>(anim.get());

      if (av->targetValue == targetRoundness)
        return av->savedValue;

      av->startValue   = av->savedValue;
      av->currentTime  = 0.0f;
      av->duration     = duration;
      av->targetValue  = targetRoundness;
      av->movementType = movementType;

      return av->savedValue;
    }
  }

  auto newAV = std::make_unique<AnimationValue<float>>(
      uiObject, uiObject->roundness, targetRoundness, duration, ROUNDNESS, movementType
  );
  float start = newAV->startValue;
  Instance->animatedObjectsValues.push_back(std::move(newAV));
  return start;
}

void Animator::Reset(UIObject *uiObject, float time) {
  if (AnimatorContains(uiObject, NONE)) return;
  for (auto &aov : Instance->animatedObjectsValues) {
    if (aov->uiObject == uiObject) {
      aov->Reset(time);
    }
  }
}

void Animator::SizeUp(UIObject *uiObject, float scale, float time) {
  Vector2 offsetSize = {uiObject->size.x * scale, uiObject->size.y * scale};
  Vector2 offsetPos = {-uiObject->size.x * scale / 2.0f,
                       -uiObject->size.y * scale / 2.0f};

  Animator::Transform(
      uiObject,
      {uiObject->position.x + offsetPos.x, uiObject->position.y + offsetPos.y},
      {uiObject->size.x + offsetSize.x, uiObject->size.y + offsetSize.y},
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
  Animator::Transform(uiObject, uiObject->position, uiObject->size, GAUSSIAN,
                      time, 0.0f, 1.0f);
}

void Animator::Free(UIObject *uiObject) {
  Instance->animatedObjectsValues.erase(
      std::remove_if(Instance->animatedObjectsValues.begin(),
                     Instance->animatedObjectsValues.end(),
                     [&](const std::unique_ptr<IAnimationValue> &anim) {
                       return anim->uiObject == uiObject;
                     }),
      Instance->animatedObjectsValues.end());
}

bool Animator::AnimatorContains(UIObject* uiObject, UIObjectParameter param) {
  for (auto& anim : Instance->animatedObjectsValues) {
    if (anim->uiObject == uiObject && anim->objectParameter == param) {
      return true;
    }
  }
  return false;
}

// #include "Animator.h"
// #include <algorithm> // find_if
//
// static Animator _;
//
// std::vector<Animator::AnimatedUIObjectsSet> Animator::animatedUIObjects;
//
// Vec2f Animator::Vector2Lerp(Vec2f start, Vec2f end, float t) {
//   return {
//     start.x + (end.x - start.x) * t,
//     start.y + (end.y - start.y) * t
//   };
// }
//
// Vec2f Animator::GetInterpolatedValue(Vec2f start, Vec2f end, float t,
//                                      MovementType movementType) {
//   float alpha = t;
//   switch (movementType) {
//   case GAUSSIAN:
//     alpha = -cos(t * PI) * .5f + .5f;
//     break;
//   case LINEAR:
//   default:
//     break;
//   }
//   return Vector2Lerp(start, end, alpha);
// }
//
// void Animator::Update() {
//   for (auto &anim : Animator::animatedUIObjects) {
//     anim.elapsed += GetFrameTime();
//     float t = anim.duration > 0.0f ? anim.elapsed / anim.duration : 1.0f;
//     if (t > 1.0f) t = 1.0f;
//
//     anim.uiObject->animOffsetPos  = GetInterpolatedValue(anim.startPosition, anim.targetPosition, t, anim.movementType);
//     anim.uiObject->animOffsetSize = GetInterpolatedValue(anim.startSize, anim.targetSize, t, anim.movementType);
//
//     float alphaT = anim.movementType == GAUSSIAN ? -cos(t * PI) * 0.5f + 0.5f : t;
//     anim.uiObject->imageAlpha = anim.startAlpha + (anim.targetAlpha - anim.startAlpha) * alphaT;
//
//     if (t >= 1.0f) {
//       anim.uiObject->animOffsetPos  = anim.targetPosition;
//       anim.uiObject->animOffsetSize = anim.targetSize;
//       anim.uiObject->imageAlpha     = anim.targetAlpha;
//     }
//   }
// }
//
// void Animator::Transform(
//   UIObject *uiObject,
//   Vec2f targetAbsPos,
//   Vec2f targetAbsSize,
//   MovementType movementType,
//   float duration,
//   float tolerance,
//   float targetImageAlpha
// ) {
//   auto it = std::find_if(animatedUIObjects.begin(), animatedUIObjects.end(),
//     [&](const AnimatedUIObjectsSet &anim) { return anim.uiObject == uiObject; });
//
//   if (it == animatedUIObjects.end()) {
//     AnimatedUIObjectsSet entry;
//     entry.uiObject   = uiObject;
//     entry.startPosition     = uiObject->position + uiObject->animOffsetPos;
//     entry.startSize         = uiObject->size + uiObject->animOffsetSize;
//     entry.targetPosition    = { targetAbsPos.x - uiObject->position.x, targetAbsPos.y - uiObject->position.y };
//     entry.targetSize        = { targetAbsSize.x - uiObject->size.x, targetAbsSize.y - uiObject->size.y };
//
//     entry.startAlpha        = uiObject->imageAlpha;
//     entry.targetAlpha       = targetImageAlpha;
//     entry.prevTargetAlpha   = targetImageAlpha;
//
//     entry.prevTargetPosition = entry.targetPosition;
//     entry.prevTargetSize     = entry.targetSize;
//
//     entry.elapsed = 0.0f;
//     entry.duration = duration;
//     entry.movementType = movementType;
//     entry.tolerance = tolerance;
//
//     animatedUIObjects.push_back(std::move(entry));
//     it = std::prev(animatedUIObjects.end());
//   }
//
//   auto &anim = *it;
//
//   //... change movement if target changed
//   bool targetChanged =
//       anim.prevTargetPosition.x != targetAbsPos.x || anim.prevTargetPosition.y != targetAbsPos.y ||
//       anim.prevTargetSize.x     != targetAbsSize.x  || anim.prevTargetSize.y     != targetAbsSize.y ||
//       anim.prevTargetAlpha      != targetImageAlpha;
//
//   if (targetChanged) {
//     anim.startPosition = uiObject->animOffsetPos;
//     anim.startSize     = uiObject->animOffsetSize;
//     anim.targetPosition = { targetAbsPos.x - uiObject->position.x, targetAbsPos.y - uiObject->position.y };
//     anim.targetSize     = { targetAbsSize.x - uiObject->size.x, targetAbsSize.y - uiObject->size.y };
//
//     anim.startAlpha     = uiObject->imageAlpha;
//     anim.targetAlpha    = targetImageAlpha;
//
//     anim.elapsed = 0.0f;
//     anim.duration = duration;
//     anim.movementType = movementType;
//     anim.tolerance = tolerance;
//
//     anim.prevTargetPosition = targetAbsPos;
//     anim.prevTargetSize     = targetAbsSize;
//     anim.prevTargetAlpha    = targetImageAlpha;
//   }
// }
//
// void Animator::SizeUp(UIObject *uiObject, float scale, float time) {
//   Vector2 offsetSize = {uiObject->size.x * scale, uiObject->size.y * scale};
//   Vector2 offsetPos = {-uiObject->size.x * scale / 2.0f,
//                        -uiObject->size.y * scale / 2.0f};
//
//   Animator::Transform(
//       uiObject,
//       {uiObject->position.x + offsetPos.x, uiObject->position.y + offsetPos.y},
//       {uiObject->size.x + offsetSize.x, uiObject->size.y + offsetSize.y},
//       GAUSSIAN, time);
// }
//
// void Animator::FadeOut(UIObject *uiObject, float time) {
//   Animator::Transform(uiObject,
//       uiObject->position,
//       uiObject->size,
//       GAUSSIAN,
//       time,
//       0.0f,
//       0.0f
//   );
// }
//
// void Animator::FadeIn(UIObject *uiObject, float time) {
//   Animator::Transform(uiObject,
//       uiObject->position,
//       uiObject->size,
//       GAUSSIAN,
//       time,
//       0.0f,
//       1.0f
//   );
// }
//
// void Animator::Reset(UIObject *uiObject, float time) {
//   Animator::Transform(uiObject,
//       uiObject->position,
//       uiObject->size,
//       GAUSSIAN, time);
// }