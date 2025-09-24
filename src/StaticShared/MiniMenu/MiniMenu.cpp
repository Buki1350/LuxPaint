#include "MiniMenu.h"

#include "../../Render/UIObjectsManager.h"
#include "../Animator/Animator.h"
#include "../FilesManager/FilesManager.h"
#include "../Utils/Utils.h"

std::vector<MiniMenu*> MiniMenu::Instances = {};

MiniMenu *MiniMenu::CreateInstance() {
  MiniMenu *newFrontPanel = new MiniMenu();
  Instances.push_back(newFrontPanel);
  return newFrontPanel;
}

void MiniMenu::DestroyInstance(MiniMenu* miniMenu) {
  if (!miniMenu || !miniMenu->_oBackground) return;
  miniMenu->_markedForDeletion = true;
}

MiniMenu *MiniMenu::Pack(UIObject *object) {
  float margin = MINIMENU_MARGIN_SCALE * Utils::GetSmallerMonitorEdge();
  if (!_oBackground) {
    _oBackground = UIObjectsManager::Create();
    _targetSize = Vec2f::ones() * margin;
    _oBackground->color = Utils::LoadColor("miniMenu");
    _oBackground->size = {0, 0};
  }

  if (object->size.x > _targetSize.x - 2 * margin) {
    _targetSize.x = object->size.x + 2 * margin;
  }

  _targetSize.y += object->size.y + margin;
  object->zLayer = _oBackground->zLayer + 1;
  object->position = _oBackground->position + _oBackground->size / 2;
  _oPackedObjects.push_back({object, object->size});
  object->size = Vec2f::zero();

  return this;
}

void MiniMenu::_HandleClosing() {
  if (Instances.size() > 0 && this == MiniMenu::Instances[0]) {
    for (auto instance : Instances) {
      if (IsMouseButtonPressed(0) && !instance->_oBackground->Clicked()) {
        DestroyInstance(instance);
        break;
      }
    }
  }
}

void MiniMenu::_CalculateTransforms() {
  if (!_oBackground || _markedForDeletion)
    return;

  float margin = MINIMENU_MARGIN_SCALE * Utils::GetSmallerMonitorEdge();

  Vec2f postAnimatedSize = Animator::AnimateSize(
      _oBackground, _targetSize, ANIMATION_POPUP_DURATION, GAUSSIAN);

  Vec2f windowSize = Utils::GetWindowSize().CastTo<float>();
  _oBackground->position = windowSize / 2 - postAnimatedSize / 2;

  float totalElementsHeight = margin;
  float maxElementWidth = 0;
  for (auto &e : _oPackedObjects) {
    totalElementsHeight += e.initialSize.y + margin;
    maxElementWidth = fmax(maxElementWidth, e.initialSize.x);
  }

  Vec2f scale = {postAnimatedSize.x / (maxElementWidth + 2 * margin),
                 postAnimatedSize.y / totalElementsHeight};

  float yOffset = margin * scale.y;
  for (auto &e : _oPackedObjects) {
    float objWidthScaled = e.initialSize.x * scale.x;
    float objHeightScaled = e.initialSize.y * scale.y;

    float x = _oBackground->position.x + margin * scale.x;
    if (centerElements) {
      x = _oBackground->position.x + (postAnimatedSize.x - objWidthScaled) / 2;
    }

    e.object->size = {objWidthScaled, objHeightScaled};
    e.object->position = {x, _oBackground->position.y + yOffset};
    yOffset += objHeightScaled + margin * scale.y;
  }
}

void MiniMenu::_HandleDeleting() {
  if (_markedForDeletion) {
    if (_deletingElapsed < _deletingDuration + ANIMATION_TOLERANCE) {
      _deletingElapsed += Utils::GetDeltaTime();
      Animator::AnimateSize(_oBackground, {0, 0}, _deletingDuration);
      Animator::AnimatePosition(_oBackground, Utils::GetWindowSize().CastTo<float>() / 2, _deletingDuration);
      for (auto &e : _oPackedObjects) {
        Animator::AnimateSize(e.object, {0, 0}, _deletingDuration);
        Animator::AnimatePosition(e.object, Utils::GetWindowSize().CastTo<float>() / 2, _deletingDuration);
      }
      return;
    }

    if (!Animator::AnimatorContains(_oBackground, NONE)) {
      auto it = std::find(Instances.begin(), Instances.end(), this);
      if (it != Instances.end()) Instances.erase(it);

      UIObjectsManager::Destroy(_oBackground);
      for (auto o : _oPackedObjects) {
        UIObjectsManager::Destroy(o.object);
      }
      _oPackedObjects.clear();

      delete this;
    }
  }
}

void MiniMenu::Update() {
  _HandleClosing();
  _CalculateTransforms();
  _HandleDeleting();
}


