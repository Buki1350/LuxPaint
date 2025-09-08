#pragma once
#include "UIObject.h"
#include <vector>

class UIObjectManager {
public:
  static std::vector<UIObject*> objects;
  static Shader roundedMask;
  static void Init();
  static UIObject* CreateUIObject(const UIObject &object);
  static UIObject* CreateUIObject();
  static void AddUIObject(UIObject *object);
  static void DrawAll();
  static UIObject* GetObjectByName(std::string name);
};
