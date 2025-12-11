#pragma once
#include "Prefabs/Button.h"
#include "Prefabs/InputField.h"
#include "UIObject.h"

#include <vector>

class UIObjectsManager {
  friend class UIObject;
  inline static bool _initialized = false;
  inline static std::vector<UIObject*> _pendingUIObjects;
  inline static std::list<UIObject*> _objectsInRenderOrder;
  static void _UpdateRenderOrderList();

public:
  static bool IsInitialized();
  static void Init();
  static void AddUIObject(UIObject *object);
  static void DrawAll();
  static UIObject* GetObjectByName(std::string name);
};
