#include "../../UI/MiniMenu/MiniMenu.h"

#include "../../Shared/Animator/Animator.h"
#include "../../Shared/FilesManager/FilesManager.h"
#include "../../Shared/KeyBindings/Keybindings.h"
#include "../../Shared/Utils/Utils.h"
#include "../../Shared/UIObjects/Extensions/UIFocusOutliner.h"
#include "../../Shared/UIObjects/UIObjectsManager.h"

std::vector<MiniMenu*> MiniMenu::instances = {};

MiniMenu *MiniMenu::CreateInstance() {
  float margin = MINIMENU_MARGIN_SCALE * Utils::View::GetSmallerMonitorEdge();
  MiniMenu *newFrontPanel = new MiniMenu();
  instances.insert(instances.begin(), newFrontPanel);
  newFrontPanel->_oBackground = new UIObject();
  newFrontPanel->_targetSize = Vec2f::ones() * margin;
  newFrontPanel->_oBackground->color = Utils::Files::LoadColor("miniMenu", "uiGlobal");
  newFrontPanel->_oBackground->size = {0, 0};
  newFrontPanel->_oBackground->color = Utils::Files::LoadColor("black");
  return newFrontPanel;
}

void MiniMenu::DestroyInstance(MiniMenu *miniMenu) {
  if (!miniMenu || !miniMenu->_oBackground)
    return;
  miniMenu->_markedForDeletion = true;
}

void MiniMenu::Destroy() { DestroyInstance(this); } 

MiniMenu * MiniMenu::Pack(UIObject* object) { return PackRow({object}); }

MiniMenu* MiniMenu::PackRow(std::initializer_list<ObjectWithSavedSize> objects) {
  float margin = MINIMENU_MARGIN_SCALE * Utils::View::GetSmallerMonitorEdge();

  Row row;
  float rowWidth = margin;
  float rowHeight = 0;

  for (auto e : objects) {
    row.push_back(e);

    if (!e.flexible) { rowWidth += e.initialSize.x; }
    rowWidth += margin;
    rowHeight = std::max(rowHeight, e.initialSize.y);

    e.object->zLayer = _oBackground->zLayer + 1;
    e.object->position = _oBackground->position + _oBackground->size / 2;
    e.object->size = Vec2f::zero();

    _oPackedObjects.push_back(e);

    if (dynamic_cast<Button*>(e.object) || dynamic_cast<InputField*>(e.object)) {
      _buttonsAndInputs.push_back(e.object);
    }
  }

  _rows.push_back(row);

  _targetSize.x = std::max(_targetSize.x, rowWidth);
  _targetSize.y += rowHeight + margin;

  return this;
}

MiniMenu::ObjectWithSavedSize MiniMenu::FlexSeparator() { return {new UIObject(), true}; }

void MiniMenu::Update() {
  _HandleClosing();
  _CalculateTransforms();
  _HandleDeleting();
  _HandleKeybindings();
}

void MiniMenu::_HandleClosing() {
  if (
    instances.empty() ||
    this != MiniMenu::instances[0] ||
    _disableClosing) return;

  for (auto instance : instances) {
    if (IsMouseButtonPressed(0)) {
      bool cursorAboveAny = instance->_oBackground->CursorAbove();

      for (auto &e : instance->_oPackedObjects) {
        if (e.object->CursorAbove()) {
          cursorAboveAny = true;
          break;
        }
      }

      if (!cursorAboveAny) {
        DestroyInstance(instance);
        break;
      }
    }
  }
}

void MiniMenu::_CalculateTransforms() {
    if (!_oBackground || _markedForDeletion)
        return;

    float margin = MINIMENU_MARGIN_SCALE * Utils::View::GetSmallerMonitorEdge();

    Vec2f postAnimatedSize = Animator::AnimateSize(
        _oBackground, _targetSize, ANIMATION_POPUP_DURATION, GAUSSIAN
    );

    Vec2f windowSize = Utils::View::GetWindowSize().CastTo<float>();
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

void MiniMenu::_HandleDeleting() {
  if (_markedForDeletion) {
    if (_deletingElapsed < _deletingDuration + ANIMATION_TOLERANCE) {
      _deletingElapsed += Utils::Time::GetDeltaTime();
      Animator::AnimateSize(_oBackground, {0, 0}, _deletingDuration);
      Animator::AnimatePosition(_oBackground, Utils::View::GetWindowSize().CastTo<float>() / 2, _deletingDuration);

      for (auto &e : _oPackedObjects) {
        Animator::AnimateSize(e.object, {0, 0}, _deletingDuration);
        Animator::AnimatePosition(e.object, Utils::View::GetWindowSize().CastTo<float>() / 2, _deletingDuration);
      }
      return;
    }

    if (!Animator::AnimatorContains(_oBackground, NONE)) {
      auto it = std::find(instances.begin(), instances.end(), this);
      if (it != instances.end()) instances.erase(it);

      std::vector<UIObject*> uiObjectsToDelete;
      uiObjectsToDelete.push_back(_oBackground);
      for (auto o : _oPackedObjects) { uiObjectsToDelete.push_back(o.object); }

      Animator::Terminate(uiObjectsToDelete);

      for (auto o : uiObjectsToDelete) { o->Destroy(); }
      _oPackedObjects.clear();
      _onDestructionFunc();

      delete this;
    }
  }
}

void MiniMenu::_HandleKeybindings() {
  if (instances.empty() || this != instances[0] || instances[0]->_buttonsAndInputs.empty()) return;

  if (Keybindings::ActionDetected(MENU_NEXT)) { _FocusNext(); }
  else if (Keybindings::ActionDetected(MENU_PREV)) { _FocusPrevious(); }
  else if (Keybindings::ActionDetected(MENU_CONFIRM)) {
    if (_currentSelected) {
      if (auto* btn = dynamic_cast<Button*>(_currentSelected)) {
        btn->Invoke();
      }
      else if (auto* input = dynamic_cast<InputField*>(_currentSelected)) {
        input->SetValue(input->GetValue() + " ");
      }
    }
  }
}

void MiniMenu::_FocusNext() {
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
    input->SetFocused(true);
  }
  else if (auto* btn = dynamic_cast<Button*>(_currentSelected)) {
    btn->SetFocused(true);
  }

  UIFocusOutliner::Focus(_currentSelected);
}

void MiniMenu::_FocusPrevious() {
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
    int prevIndex = (currentIndex - 1 + _buttonsAndInputs.size()) % _buttonsAndInputs.size();
    _currentSelected = _buttonsAndInputs[prevIndex];
  }

  if (auto* input = dynamic_cast<InputField*>(_currentSelected)) {
    input->SetFocused(true);
  }
  else if (auto* btn = dynamic_cast<Button*>(_currentSelected)) {
    btn->SetFocused(true);
  }

  UIFocusOutliner::Focus(_currentSelected);
}

void MiniMenu::OnDestroy(std::function<void()> labdaFunction) {
  _onDestructionFunc = labdaFunction;
}

void MiniMenu::DisableClosing() {
  _disableClosing = true;
}

void MiniMenu::EnableClosing() { _disableClosing = false; }

void MiniMenu::SetBackgroundColorForAll(Color color) {
  for (auto i : instances) {
    i->_oBackground->color = color;
  }
}


