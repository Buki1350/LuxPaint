#include "Updatable.h"
#include "UpdatablesManager.h"

void UpdatablesManager::UpdateAll() {
  for (Updatable* u : updatables) {
    auto copy = updatables;
    if (!u->markedForDeletion)
      u->Update();
    else
      updatables.erase(std::remove(updatables.begin(), updatables.end(), u), updatables.end());
  }
}