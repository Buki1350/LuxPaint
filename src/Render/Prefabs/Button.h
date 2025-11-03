#pragma once
#include "../../Automatition/Updatables/Updatable.h"
#include "../../StaticShared/Animator/Animator.h"
#include "../UIObject.h"

#include <functional>
class Button final : public UIObject, public Updatable {
  std::function<void()> _onClickCallback;
  bool _focused = false;
  bool _lockFocus = false; // for keybindings
public:
  bool allowInteraction = true;
  void SetFocused(bool value) {
    _focused = value;
    _lockFocus = value;
  }

  void Update() override {
    if (!allowInteraction) return;
    if (CursorAbove()) { _focused = true; }
    else if (!_lockFocus) { _focused = false; }

    if (_focused) {
      Animator::SizeUp(this);
      if (IsMouseButtonPressed(0)) Animator::AnimateColor(this, Utils::DarkenColor(color, .3f), ANIMATION_SIZEUP_DURATION);
    }
    else
      Animator::Reset(this);

    if (Clicked() && _onClickCallback != nullptr) _onClickCallback();
  };

  void OnClick(std::function<void()> callback) { _onClickCallback = callback; }

  void Invoke() {
    if (_onClickCallback != nullptr) _onClickCallback();
  }
};