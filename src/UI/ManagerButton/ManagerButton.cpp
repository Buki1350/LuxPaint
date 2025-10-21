//
// Created by lukas on 31.08.2025.
//

#include "ManagerButton.h"

#include "../../../otherLibs/nfd/nfd.h"
#include "../../App.h"
#include "../../Render/UIObjectsManager.h"
#include "../../StaticShared/Animator/Animator.h"
#include "../../StaticShared/DelayedAction/DelayedAction.h"
#include "../../StaticShared/FilesManager/FilesManager.h"
#include "../../StaticShared/Utils/Utils.h"
#include "BlankCanvasBuilder.h"

void ManagerButton::Init() {
  _uiObject_base = new UIObject(); //UIObjectsManager::Create();
  _uiObject_base->roundness = _roundness;
  _uiObject_base->color = Utils::LoadColor("managerButton");
  _uiObject_base->SetImage(FilesManager::LoadImage("manager_button.png"));
  _uiObject_base->zLayer = 2;
  _uiObject_base->imageMarginScale = UIOBJECT_ICON_MARGIN;

  _uiObject_newButton = new Button();//UIObjectsManager::CreateButton();
  _uiObject_loadButton = new Button();//UIObjectsManager::CreateButton();
  _uiObject_saveButton = new Button();//UIObjectsManager::CreateButton();
  _uiObject_settingsButton = new Button();//UIObjectsManager::CreateButton();

  Color buttonsColor = Utils::LoadColor("managerButton_listButton");

  _uiObject_newButton->color = buttonsColor;
  _uiObject_newButton->text = "New";
  _uiObject_newButton->text.center = true;
  _uiObject_newButton->OnClick([](){BlankCanvasBuilder::Build();});
  _uiObject_newButton->allowInteraction = false;

  _uiObject_loadButton->isActive = false;
  _uiObject_loadButton->color = buttonsColor;
  _uiObject_loadButton->text = "Load image";
  _uiObject_loadButton->text.center = true;
  _uiObject_loadButton->OnClick([this](){_LoadImageFromSystem();});
  _uiObject_loadButton->allowInteraction = false;

  _uiObject_saveButton->isActive = false;
  _uiObject_saveButton->color = buttonsColor;
  _uiObject_saveButton->text = "Save image";
  _uiObject_saveButton->text.center = true;
  _uiObject_saveButton->allowInteraction = false;

  _uiObject_settingsButton->isActive = false;
  _uiObject_settingsButton->color = buttonsColor;
  _uiObject_settingsButton->text = "Settings";
  _uiObject_settingsButton->text.center = true;
  _uiObject_settingsButton->isActive = false;
  _uiObject_settingsButton->allowInteraction = false;

  _uiObject_buttons.push_back(_uiObject_newButton);
  _uiObject_buttons.push_back(_uiObject_loadButton);
  _uiObject_buttons.push_back(_uiObject_saveButton);
  _uiObject_buttons.push_back(_uiObject_settingsButton);

  for (auto button : _uiObject_buttons) {
    button->zLayer = _uiObject_base->zLayer + 1;
  }
}

void ManagerButton::Update() {
  if (_uiObject_base->Clicked()) { _listExpanded = true; }
  if (!_uiObject_base->CursorAbove()) { _listExpanded = false; }
  _AdjustSizeAndPosition();
}

void ManagerButton::_AdjustSizeAndPosition() {
  //... data
  Vec2f monitorSize     = Utils::GetCurrentMonitorSize().CastTo<float>();
  Vec2f windowSize      = Utils::GetWindowSize().CastTo<float>();
  float smallerEdge          = Utils::GetSmallerMonitorEdge();
  float listButtonSeparation = smallerEdge * _listElementSeparationScale;
  float listButtonHeight     = smallerEdge * _listElementHeightScale;
  float listButtonWidth      = smallerEdge * _listElementWidthScale;
  float baseButtonMargin     = smallerEdge * _marginScale;
  float listWidth            = smallerEdge * _listElementWidthScale;
  float listHeight           = smallerEdge * (_uiObject_buttons.size() * (_listElementHeightScale + _listElementSeparationScale) + _listElementSeparationScale);
  Vec2f listSize           = { listWidth, listHeight };
  Vec2f listPos            = { windowSize.x - listSize.x - baseButtonMargin, baseButtonMargin };

  //... base
  _uiObject_base->size = {monitorSize.x * _sizeScale, monitorSize.x * _sizeScale};
  _uiObject_base->position = {
    windowSize.x - _uiObject_base->size.x - baseButtonMargin,
    baseButtonMargin
  };

  //... buttons interactions
  // before others, cuz "isActive" overrides click
  // if (_uiObject_newButton->Clicked()) BlankCanvasBuilder::Build();
  // else if (_uiObject_loadButton->Clicked()) _LoadImageFromSystem();

  //... interaction handling
  Vec2f postAnimatedPosition = _uiObject_base->position;
  Vec2f postAnimatedSize = _uiObject_base->size;
  if (_listExpanded) {
    float animationTime = 0.15f;
    postAnimatedPosition  = Animator::AnimatePosition(_uiObject_base, listPos, animationTime, GAUSSIAN);
    postAnimatedSize      = Animator::AnimateSize(_uiObject_base, listSize, animationTime, GAUSSIAN);

    float sizeScale = _uiObject_base->size.x / postAnimatedSize.x;
    Animator::AnimateRoundness(_uiObject_base, _uiObject_base->roundness * sizeScale, animationTime, GAUSSIAN);
    Animator::AnimateImageAlpha(_uiObject_base, 0, animationTime, GAUSSIAN);

    for (auto button : _uiObject_buttons) {
      button->isActive = true;
    }

    new DelayedAction(animationTime, [this]() {
      for (auto button : _uiObject_buttons) {
        button->allowInteraction = true;
      }
    });
  }
  else if (_uiObject_base->CursorAbove()) {
    Animator::SizeUp(_uiObject_base);
  }
  else {
    Animator::Reset(_uiObject_base);
    for (auto button : _uiObject_buttons) { button->isActive = false; button->allowInteraction = false; }
  }


  //... buttons
  // scaling buttons to match manager button size
  int buttonCount = _uiObject_buttons.size();
  float totalHeight = postAnimatedSize.y;
  float buttonHeight = (totalHeight - (buttonCount + 1) * listButtonSeparation) / buttonCount;
  Vec2f buttonSize = {
    postAnimatedSize.x - listButtonSeparation * 2,
    buttonHeight
  };

  for (int i = 0; i < buttonCount; i++) {
    auto button = _uiObject_buttons[i];
    button->position = {
      postAnimatedPosition.x + listButtonSeparation,
      postAnimatedPosition.y + listButtonSeparation + (buttonHeight + listButtonSeparation) * i
    };
    button->size = buttonSize;
  }

}

void ManagerButton::_LoadImageFromSystem() {
  char* outPath = nullptr;

  nfdu8filteritem_t filters[2] = {
    { "ImageManager files", "png,jpg" },
    { "All files", "*" }
  };

  nfdresult_t result = NFD_OpenDialog(
      &outPath,       // <- wyjście
      filters,        // <- lista filtrów
      2,              // <- liczba filtrów
      nullptr         // <- folder startowy (NULL = domyślny)
  );

  if (result == NFD_OKAY) {
    Image img = LoadImage(outPath);
    Texture2D tex = LoadTextureFromImage(img);
    App::Instance->canvas.AddImage(img);
    UnloadImage(img);

    NFD_FreePath(outPath); // pamiętaj o zwolnieniu
  }
  else if (result == NFD_CANCEL) {
    // użytkownik anulował
  }
  else {
    printf("NFD Error: %s\n", NFD_GetError());
  }
}