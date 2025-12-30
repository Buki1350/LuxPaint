#pragma once
#include <vector>

class Updatable;

class UpdatablesManager final {
  friend class Updatable;
  inline static std::vector<Updatable*> _updatables;
public:
  static void updateAll();
};