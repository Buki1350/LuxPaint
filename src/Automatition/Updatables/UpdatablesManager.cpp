#include "UpdatablesManager.h"
#include "Updatable.h"
#include <algorithm>
#include <iostream>

void UpdatablesManager::updateAll() {
  _updatables.erase(
      std::remove_if(
          _updatables.begin(),
          _updatables.end(),
          [](Updatable* u) { return u == nullptr || u->markedForDeletion; }
      ),
      _updatables.end()
  );

  auto snapshot = _updatables;
  for (Updatable* updatable : snapshot) {
    if (updatable != nullptr && !updatable->markedForDeletion) {
      //std::cout << "Update from: " << typeid(*updatable).name() << std::endl;
      updatable->update();
    }
  }
}
