#pragma once
#include "../../Automatition/Updatables/Updatable.h"
#include "../../StaticShared/Animator/Animator.h"
#include "../UIObject.h"

#include <functional>
class Button final : public UIObject, public Updatable {
  std::function<void()> _onClickCallback;

public:
  bool allowInteraction = true;

  void Update() override {
    if (!allowInteraction) return;
    if (CursorAbove() && !Clicked()) {
      Animator::SizeUp(this);
      if (IsMouseButtonPressed(0)) Animator::AnimateColor(this, Utils::DarkenColor(color, .3f), ANIMATION_SIZEUP_DURATION);
    }
    else
      Animator::Reset(this);

    if (Clicked() && _onClickCallback != nullptr) _onClickCallback();
  };

  void OnClick(std::function<void()> callback) { _onClickCallback = callback; }
};