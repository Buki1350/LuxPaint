#include "Animator.h"
#include "AnimationValue.h"

#include <algorithm> // find_if
#include <iostream>

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

  //std::cout << "[Animator] AOV count: " << animatedObjectsValues.size() << std::endl;
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

float Animator::AnimateOutline(UIObject *uiObject, float targetOutline,
                               float duration, MovementType movementType) {
  for (auto &anim : Instance->animatedObjectsValues) {
    if (anim->uiObject == uiObject && anim->objectParameter == OUTLINE) {
      auto av = static_cast<AnimationValue<float> *>(anim.get());

      if (av->targetValue == targetOutline)
        return av->savedValue;

      av->startValue = av->savedValue;
      av->currentTime = 0.0f;
      av->duration = duration;
      av->targetValue = targetOutline;
      av->movementType = movementType;

      return av->savedValue;
    }
  }

  auto newAV = std::make_unique<AnimationValue<float>>(
      uiObject, uiObject->outlineScale, targetOutline, duration, OUTLINE,
      movementType);
  float start = newAV->startValue;
  Instance->animatedObjectsValues.push_back(std::move(newAV));
  return start;
}

void Animator::Reset(UIObject* uiObject, UIObjectParameter parameter, float time) {
  if (Instance == nullptr) return;

  for (auto& aov : Instance->animatedObjectsValues) {
    if (aov->uiObject == uiObject && aov->objectParameter == parameter) {
      aov->Reset(time);
    }
  }
}


void Animator::Reset(UIObject *uiObject, float time) {
  if (AnimatorContains(uiObject, NONE))
    return;
  for (auto &aov : Instance->animatedObjectsValues) {
    if (aov->uiObject == uiObject) {
      aov->Reset(time);
    }
  }
}

void Animator::Apply(UIObject* uiObject) {
    // jeśli animator nie istnieje lub brak obiektów — nic nie robimy
    if (Instance == nullptr) return;

    for (auto &anim : Instance->animatedObjectsValues) {
        if (anim->uiObject == uiObject) {
            // typ animacji zależy od parametru — ale zawsze możemy użyć targetValue
            if (anim->objectParameter == POSITION) {
                auto av = static_cast<AnimationValue<Vec2f>*>(anim.get());
                *av->currentValue = av->targetValue;
                av->savedValue = av->targetValue;
            } else if (anim->objectParameter == SIZE) {
                auto av = static_cast<AnimationValue<Vec2f>*>(anim.get());
                *av->currentValue = av->targetValue;
                av->savedValue = av->targetValue;
            } else if (anim->objectParameter == COLOR) {
                auto av = static_cast<AnimationValue<Color>*>(anim.get());
                *av->currentValue = av->targetValue;
                av->savedValue = av->targetValue;
            } else if (anim->objectParameter == IMAGE_ALPHA) {
                auto av = static_cast<AnimationValue<float>*>(anim.get());
                *av->currentValue = av->targetValue;
                av->savedValue = av->targetValue;
            } else if (anim->objectParameter == ROUNDNESS) {
                auto av = static_cast<AnimationValue<float>*>(anim.get());
                *av->currentValue = av->targetValue;
                av->savedValue = av->targetValue;
            }

            anim->readyToDelete = true;
        }
    }

    // usuń animacje, które już zostały „zastosowane”
    Instance->animatedObjectsValues.erase(
        std::remove_if(
            Instance->animatedObjectsValues.begin(),
            Instance->animatedObjectsValues.end(),
            [&](const std::unique_ptr<IAnimationValue>& anim) {
                return anim->uiObject == uiObject && anim->readyToDelete;
            }
        ),
        Instance->animatedObjectsValues.end()
    );
}

void Animator::SizeUp(UIObject *uiObject, float scale, float time) {
  if (AnimatorContains(uiObject, SIZE)) return;
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
void Animator::Terminate(UIObject *uiObject) {
  auto arr = std::vector<UIObject*>{uiObject};
  Animator::Terminate(arr);
}

void Animator::Terminate(std::vector<UIObject*>& uiObjects) {
  if (Instance == nullptr) return;

  Instance->animatedObjectsValues.erase(
      std::remove_if(
          Instance->animatedObjectsValues.begin(),
          Instance->animatedObjectsValues.end(),
          [&](const std::unique_ptr<IAnimationValue>& anim) {
              return std::find(uiObjects.begin(), uiObjects.end(), anim->uiObject) != uiObjects.end();
          }
      ),
      Instance->animatedObjectsValues.end()
  );
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