#include "UpdatablesManager.h"
#include "Updatable.h"
#include <algorithm>

void UpdatablesManager::UpdateAll() {
  updatables.erase(
      std::remove_if(
          updatables.begin(),
          updatables.end(),
          [](Updatable* u) { return u == nullptr || u->markedForDeletion; }
      ),
      updatables.end()
  );

  auto snapshot = updatables;

  for (Updatable* updatable : snapshot) {
    auto s = updatable;
    if (updatable != nullptr && !updatable->markedForDeletion) {
      updatable->update();
    }
  }
}
