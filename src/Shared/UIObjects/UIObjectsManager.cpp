#include "UIObjectsManager.h"

void UIObjectsManager::_updateRenderOrderList() {
  if (_pendingUIObjects.empty()) return;

  for (UIObject* obj : _pendingUIObjects) {
    auto it = _objectsInRenderOrder.begin();
    for (; it != _objectsInRenderOrder.end(); ++it) {
      if ((*it)->getZLayer() > obj->getZLayer()) break;
    }
    _objectsInRenderOrder.insert(it, obj);
  }

  _pendingUIObjects.clear();
}


bool UIObjectsManager::isInitialized() {
  return _initialized;
}

void UIObjectsManager::init() {
  _pendingUIObjects = std::vector<UIObject*>();
  _objectsInRenderOrder = std::list<UIObject*>();

  _initialized = true;
}

void UIObjectsManager::AddUIObject(UIObject *object) {
  _pendingUIObjects.push_back(object);
}

void UIObjectsManager::drawAll() {
  _updateRenderOrderList();

  auto snapshot = _objectsInRenderOrder;
  for (UIObject* obj : _objectsInRenderOrder) {
    if (obj->isActive)
      obj->draw();
  }
}

UIObject *UIObjectsManager::GetObjectByName(std::string name) {
  for (auto &object : _objectsInRenderOrder) {
    if (object->name == name) return object;
  }
  return nullptr;
}