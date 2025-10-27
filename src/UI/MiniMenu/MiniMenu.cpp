#include "../../UI/MiniMenu/MiniMenu.h"

#include "../../Render/UIObjectsManager.h"
#include "../../StaticShared/Animator/Animator.h"
#include "../../StaticShared/FilesManager/FilesManager.h"
#include "../../StaticShared/Utils/Utils.h"

std::vector<MiniMenu*> MiniMenu::Instances = {};

MiniMenu *MiniMenu::CreateInstance() {
  MiniMenu *newFrontPanel = new MiniMenu();
  Instances.push_back(newFrontPanel);
  return newFrontPanel;
}

void MiniMenu::DestroyInstance(MiniMenu* miniMenu) {
  if (!miniMenu || !miniMenu->oBackground) return;
  miniMenu->_markedForDeletion = true;
}

MiniMenu* MiniMenu::Pack(UIObject* object) {
  return PackRow({object});
}

MiniMenu* MiniMenu::PackRow(std::initializer_list<ObjectWithSavedSize> objects) {
  float margin = MINIMENU_MARGIN_SCALE * Utils::GetSmallerMonitorEdge();

  if (!oBackground) {
    oBackground = new UIObject();
    _targetSize = Vec2f::ones() * margin;
    oBackground->color = Utils::LoadColor("miniMenu");
    oBackground->size = {0, 0};
  }

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

    e.object->zLayer = oBackground->zLayer + 1;
    e.object->position = oBackground->position + oBackground->size / 2;
    e.object->size = Vec2f::zero();

    _oPackedObjects.push_back(e);
  }

  _rows.push_back(row);

  _targetSize.x = std::max(_targetSize.x, rowWidth);
  _targetSize.y += rowHeight + margin;

  return this;
}

MiniMenu::ObjectWithSavedSize MiniMenu::FlexSeparator() {
  return {new UIObject(), true};
}

void MiniMenu::_HandleClosing() {
  if (Instances.empty() || this != MiniMenu::Instances[0]) return;

  for (auto instance : Instances) {
    if (IsMouseButtonPressed(0)) {
      bool cursorAboveAny = instance->oBackground->CursorAbove();

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
    if (!oBackground || _markedForDeletion)
        return;

    float margin = MINIMENU_MARGIN_SCALE * Utils::GetSmallerMonitorEdge();

    Vec2f postAnimatedSize = Animator::AnimateSize(
        oBackground, _targetSize, ANIMATION_POPUP_DURATION, GAUSSIAN
    );

    Vec2f windowSize = Utils::GetWindowSize().CastTo<float>();
    oBackground->position = windowSize / 2 - postAnimatedSize / 2;

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

        float xOffset = oBackground->position.x + margin * scale.x;
        if (centerElements) {
            xOffset = oBackground->position.x + (postAnimatedSize.x - rowWidthScaled) / 2 + margin * scale.x;
        }

        float extraSpace = (postAnimatedSize.x - rowWidths[r] * scale.x);
        float flexUnit = (flexCounts[r] > 0) ? extraSpace / flexCounts[r] : 0;

        for (auto &e : row) {
            float objWidthScaled = e.flexible ? flexUnit : e.initialSize.x * scale.x;
            float objHeightScaled = e.initialSize.y * scale.y;

            e.object->size = {objWidthScaled, objHeightScaled};
            e.object->position = {xOffset, oBackground->position.y + yOffset};

            xOffset += objWidthScaled + margin * scale.x;
        }

        yOffset += rowHeightScaled + margin * scale.y;
    }
}

void MiniMenu::_HandleDeleting() {
  if (_markedForDeletion) {
    if (_deletingElapsed < _deletingDuration + ANIMATION_TOLERANCE) {
      _deletingElapsed += Utils::GetDeltaTime();
      Animator::AnimateSize(oBackground, {0, 0}, _deletingDuration);
      Animator::AnimatePosition(oBackground, Utils::GetWindowSize().CastTo<float>() / 2, _deletingDuration);

      for (auto &e : _oPackedObjects) {
        Animator::AnimateSize(e.object, {0, 0}, _deletingDuration);
        Animator::AnimatePosition(e.object, Utils::GetWindowSize().CastTo<float>() / 2, _deletingDuration);
      }
      return;
    }

    if (!Animator::AnimatorContains(oBackground, NONE)) {
      auto it = std::find(Instances.begin(), Instances.end(), this);
      if (it != Instances.end()) Instances.erase(it);

      std::vector<UIObject*> uiObjectsToDelete;
      uiObjectsToDelete.push_back(oBackground);
      for (auto o : _oPackedObjects) { uiObjectsToDelete.push_back(o.object); }

      Animator::Terminate(uiObjectsToDelete);

      for (auto o : uiObjectsToDelete) { o->Destroy(); }
      _oPackedObjects.clear();
      _onDestructionFunc();

      delete this;
    }
  }
}

void MiniMenu::Update() {
  _HandleClosing();
  _CalculateTransforms();
  _HandleDeleting();
}

void MiniMenu::OnDestroy(std::function<void()> labdaFunction) {
  _onDestructionFunc = labdaFunction;
}


