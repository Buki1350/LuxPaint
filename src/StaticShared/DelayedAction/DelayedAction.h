#pragma once
#include <functional>
#include <thread>
#include <chrono>
#include <mutex>
#include <vector>

class DelayedAction final : public Updatable {
  inline static std::vector<DelayedAction*> _actions;
  float _timeLeft = -1;
  std::function<void()> _callback;
public:
  DelayedAction(float timeInSeconds, std::function<void()> action, bool unique = true) : Updatable(), _timeLeft(timeInSeconds), _callback(action) {
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
    _timeLeft -= Utils::GetDeltaTime();
    if (_timeLeft <= 0) {
      _callback();
      markedForDeletion = true;
    }
  }
};