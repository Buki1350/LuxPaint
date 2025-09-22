#include "UIObjectManager.h"

std::vector<UIObject*> UIObjectManager::objects;
Shader UIObjectManager::roundedMask;

void UIObjectManager::Init() {
  objects = std::vector<UIObject*>();
  roundedMask = LoadShader(nullptr, "../Shaders/rounded_mask.fs");
}

UIObject* UIObjectManager::CreateUIObject(const UIObject &object) {
  UIObject* objPtr = new UIObject(object);
  objects.push_back(objPtr);
  return objPtr;
}

UIObject *UIObjectManager::CreateUIObject() {
  UIObject *objPtr = new UIObject();
  objects.push_back(objPtr);
  return objPtr;
}
void UIObjectManager::AddUIObject(UIObject *object) {
  objects.push_back(object);
}

void UIObjectManager::DrawAll() {
  int objectDrawn = 0;
  int currentZLayer = 0;
  while (objectDrawn < objects.size()) {
    for (const auto o : objects) {
      if (currentZLayer == o->zLayer) {
        if (o->isActive) { o->Draw(); }
        objectDrawn++;
      }
    }
    currentZLayer++;
  }
}
UIObject *UIObjectManager::GetObjectByName(std::string name) {
  for (auto &object : objects) {
    if (object->name == name) return object;
  }
  return nullptr;
}