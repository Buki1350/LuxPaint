#pragma once
#include "Shared/UIObjects/UIObject.h"
#include "Snapshot.h"
#include "raylib.h"

#include <vector>

class UIObject;
class ImageSnapshot final : public Snapshot {
  std::vector<std::pair<UIObject *, Image>> _canvasLayersSnapshot;

public:
  ImageSnapshot(const std::vector<UIObject*>& canvasLayersSnapshot) {
    for (auto* uiObject : canvasLayersSnapshot) {
      Image src = uiObject->getImage();

      if (src.data) {
        Image copy = ImageCopy(src);
        _canvasLayersSnapshot.emplace_back(uiObject, copy);
      }
    }
  }

  ~ImageSnapshot() override {
    for (auto& pair : _canvasLayersSnapshot) {
      if (pair.second.data) {
        UnloadImage(pair.second);
      }
    }
  }

  void jumpTo() override {
    for (auto& pair : _canvasLayersSnapshot) {
      pair.first->setImage(pair.second);
    }
  }
};
