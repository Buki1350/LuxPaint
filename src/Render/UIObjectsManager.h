#pragma once
#include "Prefabs/Button.h"
#include "Prefabs/InputField.h"
#include "UIObject.h"

#include <vector>

class UIObjectsManager {
  friend class UIObject;
  static std::vector<UIObject*> _objects;
  inline static bool _initialized = false;
public:
  static bool IsInitialized();
  static Shader roundedMask;
  static void Init();
  static void AddUIObject(UIObject *object);
  static void DrawAll();
  static UIObject* GetObjectByName(std::string name);
};
