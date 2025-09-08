#include "Updatables.h"
#include "Updatable.h"

std::vector<Updatable*> updatables;

void Updatables::UpdateAll() {
  for (Updatable* u : updatables) { u->Update(); }
}