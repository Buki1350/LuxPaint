#include "Updatable.h"
#include "UpdatablesManager.h"

void UpdatablesManager::UpdateAll() {
  for (Updatable* u : updatables) { u->Update(); }
}