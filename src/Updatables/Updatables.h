#pragma once
#include <vector>

class Updatable;

class Updatables {
public:
  inline static std::vector<Updatable*> updatables;
  static void UpdateAll();
};