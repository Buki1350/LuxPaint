#include "UIObjectsManager.h"

std::vector<UIObject*> UIObjectsManager::_objects;
Shader UIObjectsManager::roundedMask;

bool UIObjectsManager::IsInitialized() {
  return _initialized;
}

void UIObjectsManager::Init() {
  _objects = std::vector<UIObject*>();
  roundedMask = LoadShader(nullptr, "../Shaders/rounded_mask.fs");
  _initialized = true;
}

void UIObjectsManager::AddUIObject(UIObject *object) {
  _objects.push_back(object);
}

void UIObjectsManager::DrawAll() {
  auto cp = _objects;
  int objectDrawn = 0;
  int currentZLayer = 0;
  while (objectDrawn < _objects.size()) {
    for (const auto o : _objects) {
      if (currentZLayer == o->zLayer) {
        if (o->isActive) { o->Draw(); }
        objectDrawn++;
      }
    }
    currentZLayer++;
  }
}
UIObject *UIObjectsManager::GetObjectByName(std::string name) {
  for (auto &object : _objects) {
    if (object->name == name) return object;
  }
  return nullptr;
}