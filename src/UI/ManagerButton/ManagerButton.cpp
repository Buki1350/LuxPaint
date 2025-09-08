//
// Created by lukas on 31.08.2025.
//

#include "ManagerButton.h"

#include "../../Render/UIObjectManager.h"
#include "../../Static/FilesManager/FilesManager.h"
#include "../../Static/Utils/Utils.h"

void ManagerButton::Init() {
  _uiObject_base = UIObjectManager::CreateUIObject();
  _uiObject_base->roundness = _roundness;
  _uiObject_base->color = Utils::LoadColor("managerButton");
  _uiObject_base->SetImage(FilesManager::LoadImage("manager_button.png"));
  _uiObject_base->zLayer = 2;

  UIObject* loadButton = UIObjectManager::CreateUIObject();
  UIObject* saveButton = UIObjectManager::CreateUIObject();
  UIObject* settingsButton = UIObjectManager::CreateUIObject();

  loadButton->color = Utils::LoadColor("managerButton_ListButton");
  loadButton->text = "Load image";
  loadButton->text.center = true;
  saveButton->color = Utils::LoadColor("managerButton_ListButton");;
  saveButton->text = "Save image";
  saveButton->text.center = true;
  settingsButton->color = Utils::LoadColor("managerButton_ListButton");;
  settingsButton->text = "Settings";
  settingsButton->text.center = true;

  _uiObject_buttons.push_back(loadButton);
  _uiObject_buttons.push_back(saveButton);
  _uiObject_buttons.push_back(settingsButton);

  for (auto button : _uiObject_buttons) {
    button->zLayer = _uiObject_base->zLayer + 1;
  }
}

void ManagerButton::Update() {
  AdjustSizeAndPosition();
  HandleClick();
}

void ManagerButton::AdjustSizeAndPosition() {
  //... data
  Vector2Int monitorSize     = Utils::GetCurrentMonitorSize();
  Vector2Int windowSize      = Utils::GetWindowSize();
  float smallerEdge          = Utils::GetSmallerMonitorEdge();
  float listButtonSeparation = smallerEdge * _listElementSeparationScale;
  float listButtonHeight     = smallerEdge * _listElementHeightScale;
  float listButtonWidth      = smallerEdge * _listElementWidthScale;
  float baseButtonMargin     = smallerEdge * _marginScale;
  float listWidth            = smallerEdge * _listElementWidthScale;
  float listHeight           = smallerEdge * (_uiObject_buttons.size() * (_listElementHeightScale + _listElementSeparationScale) + _listElementSeparationScale);
  Vector2 listSize           = { listWidth, listHeight };
  Vector2 listPos            = { windowSize.x - listSize.x - baseButtonMargin, baseButtonMargin };

  //... base
  _uiObject_base->size = {monitorSize.x * _sizeScale, monitorSize.x * _sizeScale};
  _uiObject_base->position = {
    windowSize.x - _uiObject_base->size.x - baseButtonMargin,
    baseButtonMargin
  };

  for (int i = 0; i < _uiObject_buttons.size(); i++) {
    _uiObject_buttons[i]->position = {
      windowSize.x - listButtonSeparation,
      _uiObject_base->position.y + _uiObject_base->animOffsetPos.y + listButtonSeparation + (listButtonSeparation + listButtonHeight) * i,
    };

    _uiObject_buttons[i]->size = { 0, listButtonHeight };
  }

  //... interaction handling
  if (_uiObject_base->IsCursorAbove() && !_listExpanded) {
    Animator::SizeUp(_uiObject_base);
  }

  float animationTime = 0.15f;
  if (_uiObject_base->IsCursorAbove() && _listExpanded) {
    //.. base
    Animator::Transform(
      _uiObject_base,
      listPos,
      listSize,
      GAUSSIAN,
      animationTime,
      ANIMATION_DEFAULT_TOLERANCE,
      0);

    // //.. other buttons
    for (int i = 0; i < _uiObject_buttons.size(); i++) {
      Animator::Transform(
        _uiObject_buttons[i],
        {listPos.x + listButtonSeparation, listPos.y + listButtonSeparation + (listButtonHeight + listButtonSeparation) * i},
        {listSize.x - listButtonSeparation * 2, _uiObject_buttons[i]->size.y},
        GAUSSIAN,
        animationTime
        );

      if (_uiObject_buttons[i]->IsCursorAbove()) {
        //Animator::SizeUp(_uiObject_buttons[i]); //TODO
      }
    }

  }
  else {
    Animator::Reset(_uiObject_base, animationTime);
    Animator::FadeIn(_uiObject_base, animationTime);
    for (auto button : _uiObject_buttons) {
      Animator::Reset(button, animationTime / 10);
    }

    _listExpanded = false;
  }
}

void ManagerButton::HandleClick() {
  if (_uiObject_base->IsClicked()) { _listExpanded = true; }
}