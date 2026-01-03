#pragma once
#include "Initializable.h"
#include <vector>

class Initializables {
public:
  static std::vector<Initializable*> initializables;

  static void addInitializable(Initializable* initializable) {
    initializables.push_back(initializable);
  }

  static void initAll() {
    for (auto i : initializables) {
      i->init();
    }
  }
};
