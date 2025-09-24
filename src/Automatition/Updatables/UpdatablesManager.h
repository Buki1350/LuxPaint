#pragma once
#include <vector>

class Updatable;

class UpdatablesManager final {
public:
  inline static std::vector<Updatable*> updatables;
  static void UpdateAll();
};