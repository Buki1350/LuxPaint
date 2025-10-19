//
// Created by lukas on 30.09.2025.
//

#include "Button.h"

#include "../../StaticShared/DelayedAction/DelayedAction.h"
void Button::Update() {
  if (!allowInteraction) return;
  if (CursorAbove() && !Clicked()) {
    Animator::SizeUp(this);
    if (IsMouseButtonPressed(0)) Animator::AnimateColor(this, Utils::DarkenColor(color, .3f), ANIMATION_SIZEUP_DURATION);
  }
  else
    Animator::Reset(this);

  if (Clicked() && _onClickCallback != nullptr) _onClickCallback();
}

void Button::OnClick(std::function<void()> callback) { _onClickCallback = callback; }