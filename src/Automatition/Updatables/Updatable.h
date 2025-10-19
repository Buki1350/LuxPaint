#pragma once
#include "UpdatablesManager.h"

class Updatable {
public:
  bool markedForDeletion = false;
  Updatable() { UpdatablesManager::updatables.push_back(this); }
  virtual ~Updatable() = default;
  virtual void Update() = 0;
};