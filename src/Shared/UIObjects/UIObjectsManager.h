#pragma once
#include "Prefabs/Button.h"
#include "Prefabs/InputField.h"
#include "UIObject.h"

#include <vector>

class UIObjectsManager {
  friend class UIObject;
  inline static bool _initialized = false;
public:
  static std::vector<UIObject*> _objects;
  static bool IsInitialized();
  static Shader roundedMask;
  static void Init();
  static void AddUIObject(UIObject *object);
  static void DrawAll();
  static UIObject* GetObjectByName(std::string name);
};
