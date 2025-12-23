#pragma once
#include "UpdatablesManager.h"

class Updatable {
public:
  bool markedForDeletion = false;
  Updatable() { UpdatablesManager::updatables.push_back(this); }
  virtual ~Updatable() = default;
  virtual void update() = 0;
};