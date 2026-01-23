#include "../../MiniMenus/MiniMenu/MiniMenu.h"

#include "../../../Shared/Animator/Animator.h"
#include "../../../Shared/Keybindings/Keybindings.h"
#include "../../../Shared/Serializer/Serializer.h"
#include "../../../Shared/UIObjects/Extensions/UIFocusOutliner.h"
#include "../../../Shared/UIObjects/UIObjectsManager.h"
#include "Shared/Utils/Files/utiFiles.h"
#include "Shared/Utils/View/utiView.h"

#include <iostream>

std::vector<MiniMenu*> MiniMenu::instances = {};

MiniMenu *MiniMenu::createInstance() {
  float margin = MINIMENU_MARGIN_SCALE * uti::view::getSmallerMonitorEdge();
  MiniMenu *newInstance = new MiniMenu();
  instances.insert(instances.begin(), newInstance);
  newInstance->_oBackground = new UIObject();
  newInstance->_targetSize = Vec2f::ones() * margin;
  newInstance->_oBackground->size = {0, 0};
  newInstance->_oBackground->color = uti::files::loadColor("miniMenu");
  if (instances.size() > 1)
    newInstance->_oBackground->setZLayer(instances[1]->_oBackground->getZLayer() + 1);

  return newInstance;
}

void MiniMenu::destroyInstance(MiniMenu *miniMenu) {
  if (!miniMenu || !miniMenu->_oBackground)
    return;
  miniMenu->_markedForDeletion = true;
}

void MiniMenu::destroy() { destroyInstance(this); } 

MiniMenu * MiniMenu::pack(UIObject* object) { return packRow({object}); }

MiniMenu *
MiniMenu::packRow(std::initializer_list<ObjectWithSavedSize> objects) {
  float margin = MINIMENU_MARGIN_SCALE * uti::view::getSmallerMonitorEdge();

  Row row;
  float rowWidth = margin;
  float rowHeight = 0;

  for (auto e : objects) {
    row.push_back(e);

    if (!e.flexible) {
      rowWidth += e.initialSize.x;
    }
    rowWidth += margin;
    rowHeight = std::max(rowHeight, e.initialSize.y);

    e.object->setZLayer(_oBackground->getZLayer() + 1);
    e.object->position = _oBackground->position + _oBackground->size / 2;
    e.object->size = Vec2f::zero();

    _oPackedObjects.push_back(e);

    if (dynamic_cast<Button *>(e.object) ||
        dynamic_cast<InputField *>(e.object)) {
      _buttonsAndInputs.push_back(e.object);
    }
  }

  _rows.push_back(row);

  _targetSize.x = std::max(_targetSize.x, rowWidth);
  _targetSize.y += rowHeight + margin;

  return this;
}
void MiniMenu::_handleDynamicFontColor() {
  Color textCol = uti::colors::getDynamicBlackOrWhiteFor(_oBackground->color);
  for (auto &el : _oPackedObjects) {
    if (auto* input = dynamic_cast<InputField*>(el.object)) {
      // input->textColor = textCol; // ... bugfix
    }
    else {
      el.object->text.textColor = textCol;
    }
  }
}

MiniMenu::ObjectWithSavedSize MiniMenu::flexSeparator() { return {new UIObject(), true}; }

void MiniMenu::update() {
  _calculateTransforms();
  _handleDynamicFontColor();
  _handleKeybindings();
  _handleDeleting();
  _handleClosing();
}

void MiniMenu::_handleClosing() {
  if (
    instances.empty() ||
    this != MiniMenu::instances[0] ||
    _disableClosing) return;

  if (IsMouseButtonPressed(0)) {
    for (auto instance : instances) {
      bool cursorAboveAny = instance->_oBackground->cursorAbove();

      for (auto &e : instance->_oPackedObjects) {
        if (e.object->cursorAbove()) {
          cursorAboveAny = true;
          break;
        }
      }

      if (!cursorAboveAny) {
        destroyInstance(instance);
        break;
      }
    }
  }
}

void MiniMenu::_calculateTransforms() {
    if (!_oBackground || _markedForDeletion) return;

    float margin = MINIMENU_MARGIN_SCALE * uti::view::getSmallerMonitorEdge();

    Vec2f postAnimatedSize = Animator::animateSize(
        _oBackground, _targetSize, ANIMATION_POPUP_DURATION, GAUSSIAN
    );

    Vec2f windowSize = uti::view::getWindowSize().CastTo<float>();
    _oBackground->position = windowSize / 2 - postAnimatedSize / 2;

    float totalHeight = margin;
    float maxRowWidth = 0;
    std::vector<float> rowHeights;
    std::vector<float> rowWidths;
    std::vector<int> flexCounts;

    for (auto &row : _rows) {
        float rowWidth = 0;
        float rowHeight = 0;
        int flexibleCount = 0;

        for (auto &e : row) {
          if (e.flexible) {
            flexibleCount++;
          } else {
            rowWidth += e.initialSize.x;
          }
          rowWidth += margin;

          rowHeight = std::max(rowHeight, e.initialSize.y);
        }
        rowWidth += margin;

        rowWidths.push_back(rowWidth);
        rowHeights.push_back(rowHeight);
        flexCounts.push_back(flexibleCount);

        totalHeight += rowHeight + margin;
        maxRowWidth = std::max(maxRowWidth, rowWidth);
    }

    Vec2f scale = {
        postAnimatedSize.x / maxRowWidth,
        postAnimatedSize.y / totalHeight
    };

    float yOffset = margin * scale.y;

    for (size_t r = 0; r < _rows.size(); r++) {
        auto &row = _rows[r];
        float rowWidthScaled = rowWidths[r] * scale.x;
        float rowHeightScaled = rowHeights[r] * scale.y;

        float xOffset = _oBackground->position.x + margin * scale.x;
        if (centerElements) {
            xOffset = _oBackground->position.x + (postAnimatedSize.x - rowWidthScaled) / 2 + margin * scale.x;
        }

        float extraSpace = (postAnimatedSize.x - rowWidths[r] * scale.x);
        float flexUnit = (flexCounts[r] > 0) ? extraSpace / flexCounts[r] : 0;

        for (auto &e : row) {
            float objWidthScaled = e.flexible ? flexUnit : e.initialSize.x * scale.x;
            float objHeightScaled = e.initialSize.y * scale.y;

            e.object->size = {objWidthScaled, objHeightScaled};
            e.object->position = {xOffset, _oBackground->position.y + yOffset};

            xOffset += objWidthScaled + margin * scale.x;
        }

        yOffset += rowHeightScaled + margin * scale.y;
    }
}

void MiniMenu::_handleDeleting() {
  if (_markedForDeletion) {
    if (_deletingElapsed < _deletingDuration + ANIMATION_TOLERANCE) {
      _deletingElapsed += uti::time::getDeltaTime();
      Animator::animateSize(_oBackground, {0, 0}, _deletingDuration);
      Animator::animatePosition(_oBackground, uti::view::getWindowSize().CastTo<float>() / 2, _deletingDuration);

      for (auto &e : _oPackedObjects) {
        Animator::animateSize(e.object, {0, 0}, _deletingDuration);
        Animator::animatePosition(e.object, uti::view::getWindowSize().CastTo<float>() / 2, _deletingDuration);
      }
      return;
    }

    if (!Animator::animatorContains(_oBackground, ANIM_NONE)) {
      auto it = std::find(instances.begin(), instances.end(), this);
      if (it != instances.end()) instances.erase(it);

      std::vector<UIObject*> uiObjectsToDelete;
      uiObjectsToDelete.push_back(_oBackground);
      for (auto o : _oPackedObjects) { uiObjectsToDelete.push_back(o.object); }

      Animator::terminate(uiObjectsToDelete);

      for (auto o : uiObjectsToDelete) { o->destroy(); }
      _oPackedObjects.clear();
      _onDestructionFunc();

      markedForDeletion = true;
    }
  }
}

void MiniMenu::_handleKeybindings() {
  if (instances.empty() || this != instances[0] || instances[0]->_buttonsAndInputs.empty()) return;

  if (Keybindings::getDetectedAction()) { _focusNext(); }
  else if (Keybindings::getDetectedAction()) { _focusPrevious(); }
  else if (Keybindings::getDetectedAction()) {
    if (_currentSelected) {
      if (auto* btn = dynamic_cast<Button*>(_currentSelected)) {
        btn->invoke();
      }
      else if (auto* input = dynamic_cast<InputField*>(_currentSelected)) {
        input->setValue(input->getValue() + " ");
      }
    }
  }
}

void MiniMenu::_focusNext() {
  int currentIndex = -1;
  if (_currentSelected == nullptr) _currentSelected = _buttonsAndInputs[0];
  else {
    for (int i = 0; i < _buttonsAndInputs.size(); i++) {
      if (_buttonsAndInputs[i] == _currentSelected) {
        currentIndex = i;
        break;
      }
    }
  }

  int nextIndex = (currentIndex + 1) % _buttonsAndInputs.size();
  _currentSelected = _buttonsAndInputs[nextIndex];

  if (auto* input = dynamic_cast<InputField*>(_currentSelected)) {
    //input->setFocused(true);
  }
  else if (auto* btn = dynamic_cast<Button*>(_currentSelected)) {
    btn->setFocused(true);
  }

  UIFocusOutliner::focus(_currentSelected);
}

void MiniMenu::_focusPrevious() {
  if (_buttonsAndInputs.empty()) return;

  int currentIndex = -1;

  for (int i = 0; i < _buttonsAndInputs.size(); i++) {
    if (_buttonsAndInputs[i] == _currentSelected) {
      currentIndex = i;
      break;
    }
  }

  if (currentIndex == -1) {
    _currentSelected = _buttonsAndInputs.back();
  }
  else {
    int prevIndex = (int)(currentIndex - 1 + _buttonsAndInputs.size()) % _buttonsAndInputs.size();
    _currentSelected = _buttonsAndInputs[prevIndex];
  }

  if (auto* input = dynamic_cast<InputField*>(_currentSelected)) {
    //input->setFocused(true);
  }
  else if (auto* btn = dynamic_cast<Button*>(_currentSelected)) {
    btn->setFocused(true);
  }

  UIFocusOutliner::focus(_currentSelected);
}

void MiniMenu::onDestroy(std::function<void()> labdaFunction) {
  _onDestructionFunc = labdaFunction;
}

void MiniMenu::disableClosing() {
  _disableClosing = true;
}

void MiniMenu::enableClosing() { _disableClosing = false; }

void MiniMenu::createSmallSeparator() {
  auto sep = new UIObject();
  sep->name = "separator";
  sep->size = { 500, 2 };
  sep->color = uti::colors::getDynamicBlackOrWhiteFor(this->_oBackground->color);
  sep->roundness = 1;
  sep->outlineScale = 0;
  this->packRow({ sep });
}

void MiniMenu::setBackgroundColorForAll(Color color) {
  for (auto i : instances) {
    i->_oBackground->color = color;
    for (auto p : i->_oPackedObjects) {
      p.object->text.textColor =
          uti::colors::getDynamicBlackOrWhiteFor(i->_oBackground->color);

      if (p.object->name == "separator" ) {
        p.object->color = uti::colors::getDynamicBlackOrWhiteFor(i->getOBackground()->color);
      }
      else if (p.object->name == "colorButtonOutline" ) {
        p.object->outlineColor = uti::colors::getDynamicBlackOrWhiteFor(i->getOBackground()->color);
      }
    }
  }
}
const UIObject *MiniMenu::getOBackground() {
  return _oBackground;
}


