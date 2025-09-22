#include "Updatables.h"
#include "Updatable.h"

void Updatables::UpdateAll() {
  for (Updatable* u : updatables) { u->Update(); }
}