#pragma once
#include <functional>
#include <vector>
#include "../../Automatition/Updatables/Updatable.h"

class DelayedAction_until final : public Updatable {
  inline static std::vector<DelayedAction_until*> _actions;
  std::function<bool()> _condition;
  std::function<void()> _callback;
public:
  DelayedAction_until(std::function<bool()> condition, std::function<void()> action, bool unique = true)
  : _condition(condition), _callback(action) {
    if (unique) {
      for (auto dA : _actions) {
        if (dA == this) {
          return;
        }
      }
    }

    _actions.push_back(this);
  }

  void Update() override {
    if (_condition()) {
      _callback();
      markedForDeletion = true;
    }
  }
};