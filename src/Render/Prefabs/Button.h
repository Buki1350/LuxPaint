#pragma once
#include "../../Automatition/Updatables/Updatable.h"
#include "../../StaticShared/Animator/Animator.h"
#include "../UIObject.h"

#include <functional>
class Button final : public UIObject, public Updatable {
  std::function<void()> _onClickCallback;
public:
  bool allowInteraction = true;
  void Update() override;
  void OnClick(std::function<void()> callback);
};