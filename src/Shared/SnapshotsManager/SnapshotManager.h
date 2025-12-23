#pragma once
#include "SnapshotsTypes/Snapshot.h"
#include <memory>
#include <vector>

class SnapshotManager {
  std::vector<std::unique_ptr<Snapshot>> _snapshots;
  int _currentIndex = 0;

  bool canUndo() { return _snapshots.size() > 1 && _currentIndex + 1 < _snapshots.size(); }
  bool canRedo() { return _currentIndex > 0; }

public:
  void saveSnapshot(std::unique_ptr<Snapshot> snapshot) {
    if (_currentIndex != 0) {
      // usuń całą "przyszłość" undo
      _snapshots.erase(
          _snapshots.begin(),
          _snapshots.begin() + _currentIndex
      );
      _currentIndex = 0;
    }

    _snapshots.insert(_snapshots.begin(), std::move(snapshot));

    if (_snapshots.size() > 50)
      _snapshots.pop_back();
  }


  void undo() {
    if (!canUndo()) return;
    _currentIndex++;
    _snapshots[_currentIndex]->jumpTo();
  }

  void redo() {
    if (!canRedo()) return;
    _currentIndex--;
    _snapshots[_currentIndex]->jumpTo();
  }
};
