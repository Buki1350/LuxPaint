#pragma once
#include "../../../Automatition/Updatables/Updatable.h"
#include "../../../Shared/Animator/Animator.h"
#include "../UIObject.h"
#include "Shared/Utils/Colors/Colors.h"
#include "Shared/Utils/Input/Input.h"

#include <functional>

class Button final : public UIObject, public Updatable {
  std::function<void()> _onClickCallback;
  bool _focused = false;
  bool _lockFocus = false; // for keybindings
  bool _darken = false;

public:
  bool allowInteraction = true;

  void setFocused(bool value) {
    _focused = value;
    _lockFocus = value;
  }

  void update() override {
    if (!allowInteraction)
      return;

    if (cursorAbove()) {
      _focused = true;
    }
    else if (!_lockFocus) {
      _focused = false;
    }
    else {
      _focused = false;
      Animator::reset(this, COLOR, ANIMATION_SIZEUP_DURATION);
    }

    if (!cursorAbove() || uti::input::mouseReleased()) {
      Animator::reset(this, ANIMATION_SIZEUP_DURATION);
      _darken = false;
    }

    if (_focused) {
      Animator::sizeUp(this);

      if (IsMouseButtonPressed(0)) {
        if (!_darken) {
          Animator::animateColor(this, uti::colors::darkenColor(color, .3f), ANIMATION_SIZEUP_DURATION);
          _darken = true;
        }
      }
    }
    else {
      Animator::reset(this);
      _darken = false;
    }

    if (clicked() && _onClickCallback != nullptr) {
      _onClickCallback();
    }
  };

  void onClick(std::function<void()> callback) { _onClickCallback = callback; }

  void invoke() {
    if (_onClickCallback != nullptr) _onClickCallback();
  }
};