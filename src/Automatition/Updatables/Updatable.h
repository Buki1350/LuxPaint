#pragma once
#include "Updatables.h"

class Updatable {
public:
  Updatable() { Updatables::updatables.push_back(this); }
  virtual ~Updatable() {
    auto& v = Updatables::updatables;
    v.erase(std::remove(v.begin(), v.end(), this), v.end());
  }
  virtual void Update() = 0;
};