#include "UpdatablesManager.h"
#include "Updatable.h"
#include <algorithm>

void UpdatablesManager::UpdateAll() {
  // 1) Usuń wskaźniki oznaczone do usunięcia (cleanup)
  updatables.erase(
      std::remove_if(
          updatables.begin(),
          updatables.end(),
          [](Updatable* u) { return u == nullptr || u->markedForDeletion; }
      ),
      updatables.end()
  );

  // 2) Zrób snapshot (kopię wskaźników) przed wywołaniem Update,
  //    żeby modyfikacje/realokacje wektora w trakcie Update nie psuły iteracji.
  auto snapshot = updatables;

  for (Updatable* u : snapshot) {
    // upewnij się, że obiekt wciąż istnieje i nie został oznaczony do usunięcia
    if (u && !u->markedForDeletion) {
      u->Update();
    }
  }
}
