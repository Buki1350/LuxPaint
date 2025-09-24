#pragma once
#include "UIObject.h"
#include <vector>

class UIObjectsManager {
public:
  static std::vector<UIObject*> objects;
  static Shader roundedMask;
  static void Init();
  static UIObject* Create(const UIObject &object);
  static UIObject* Create();
  static void Destroy(UIObject* object);
  static void AddUIObject(UIObject *object);
  static void DrawAll();
  static UIObject* GetObjectByName(std::string name);
};
