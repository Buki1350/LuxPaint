#pragma once
#include <functional>
#include <vector>
#include "../../Automatition/Updatables/Updatable.h"

class DelayedAction_inSeconds final : public Updatable {
  inline static std::vector<DelayedAction_inSeconds*> _actions;
  float _timeLeft = -1;
  std::function<void()> _callback;
public:
  DelayedAction_inSeconds(float timeInSeconds, std::function<void()> action, bool unique = true)
  : Updatable(), _timeLeft(timeInSeconds), _callback(action) {
    if (unique) {
      for (auto dA : _actions) {
        if (dA == this) {
          return;
        }
      }
    }

    _actions.push_back(this);
  }

  void update() override {
    _timeLeft -= uti::time::getDeltaTime();
    if (_timeLeft <= 0) {
      _callback();
      markedForDeletion = true;
    }
  }
};

