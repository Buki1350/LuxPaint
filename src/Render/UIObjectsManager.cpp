#include "UIObjectsManager.h"

std::vector<UIObject*> UIObjectsManager::objects;
Shader UIObjectsManager::roundedMask;

void UIObjectsManager::Init() {
  objects = std::vector<UIObject*>();
  roundedMask = LoadShader(nullptr, "../Shaders/rounded_mask.fs");
}

UIObject* UIObjectsManager::Create(const UIObject &object) {
  UIObject* objPtr = new UIObject(object);
  objects.push_back(objPtr);
  return objPtr;
}

UIObject *UIObjectsManager::Create() {
  UIObject *objPtr = new UIObject();
  objects.push_back(objPtr);
  return objPtr;
}

InputField *UIObjectsManager::CreateInputField() {
  InputField *objPtr = new InputField();
  objects.push_back(objPtr);
  return objPtr;
}

Button *UIObjectsManager::CreateButton() {
  Button *objPtr = new Button();
  objects.push_back(objPtr);
  return objPtr;
}

void UIObjectsManager::Destroy(UIObject *object) {
  Animator::Terminate(object);
  for (auto it = objects.begin(); it != objects.end(); ++it) {
    if (*it == object) {
      objects.erase(it);
      break;
    }
  }

  if (auto up = dynamic_cast<Updatable*>(object)) {
    auto &vec = UpdatablesManager::updatables;
    vec.erase(std::remove(vec.begin(), vec.end(), up), vec.end());
  }

  delete object;
}



void UIObjectsManager::AddUIObject(UIObject *object) {
  objects.push_back(object);
}

void UIObjectsManager::DrawAll() {
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
UIObject *UIObjectsManager::GetObjectByName(std::string name) {
  for (auto &object : objects) {
    if (object->name == name) return object;
  }
  return nullptr;
}