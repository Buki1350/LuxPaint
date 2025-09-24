#pragma once
#include "UpdatablesManager.h"

class Updatable {
public:
  Updatable() { UpdatablesManager::updatables.push_back(this); }
  virtual ~Updatable() {
    auto& v = UpdatablesManager::updatables;
    v.erase(std::remove(v.begin(), v.end(), this), v.end());
  }
  virtual void Update() = 0;
};