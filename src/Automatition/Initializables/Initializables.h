#pragma once
#include "Initializable.h"
#include <vector>

class Initializables {
public:
  static std::vector<Initializable*> initializables;

  static void AddInitializable(Initializable* initializable) {
    initializables.push_back(initializable);
  }

  static void InitAll() {
    for (auto i : initializables) {
      i->init();
    }
  }
};
