#pragma once

#include "../../Render/UIObject.h"
#include "../../Automatition/Updatables/Updatable.h"
#include <vector>


class MiniMenu final : public Updatable {

  // for popup animation - size up from point in the middle
  struct ObjectWithSavedSize {
    UIObject* object;
    Vec2f initialSize;
  };
  Vec2f _targetSize;

  UIObject* _oBackground = nullptr;
  std::vector<ObjectWithSavedSize> _oPackedObjects;

  // same time as popup, but time is saved to delete pointer after animation
  const float _deletingDuration = ANIMATION_POPUP_DURATION;
  float _deletingElapsed = 0.0f;
  bool _markedForDeletion = false;

  public:
  bool centerElements = false;
  static std::vector<MiniMenu*> Instances;
  static MiniMenu* CreateInstance();
  static void DestroyInstance(MiniMenu* miniMenu);

  MiniMenu *Pack(UIObject *object);
  void _HandleClosing();
  void _CalculateTransforms();
  void _HandleDeleting();
  void Update() override;
};