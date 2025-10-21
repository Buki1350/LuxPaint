#pragma once
#include "Prefabs/Button.h"
#include "Prefabs/InputField.h"
#include "UIObject.h"

#include <vector>

class UIObjectsManager {
public:
  static std::vector<UIObject*> objects;
  static Shader roundedMask;
  static void Init();
  static void AddUIObject(UIObject *object);
  static void DrawAll();
  static UIObject* GetObjectByName(std::string name);
};
