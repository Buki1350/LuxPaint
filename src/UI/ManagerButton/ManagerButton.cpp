//
// Created by lukas on 31.08.2025.
//

#include "ManagerButton.h"

#include "../../../otherLibs/nfd/nfd.h"
#include "../../App.h"
#include "../../Render/UIObjectsManager.h"
#include "../../StaticShared/Animator/Animator.h"
#include "../../StaticShared/DelayedAction/DelayedAction_inSeconds.h"
#include "../../StaticShared/FilesManager/FilesManager.h"
#include "../../StaticShared/Utils/Utils.h"
#include "BlankCanvasBuilder.h"
#include "SettingsBuilder.h"

void ManagerButton::Init() {
  _oBackground = new UIObject(); //UIObjectsManager::Create();
  _oBackground->roundness = _roundness;
  _oBackground->color = Utils::Files::LoadColor("managerButton");
  _oBackground->SetImage(FilesManager::LoadImage("manager_button.png"));
  _oBackground->zLayer = 2;
  _oBackground->imageMarginScale = UIOBJECT_ICON_MARGIN;

  _oNewButton = new Button();//UIObjectsManager::CreateButton();
  _oLoadButton = new Button();//UIObjectsManager::CreateButton();
  _oSaveButton = new Button();//UIObjectsManager::CreateButton();
  _oSettingsButton = new Button();//UIObjectsManager::CreateButton();

  Color buttonsColor = Utils::Files::LoadColor("managerButton_listButton");

  _oNewButton->color = buttonsColor;
  _oNewButton->text = "New";
  _oNewButton->text.center = true;
  _oNewButton->OnClick([](){BlankCanvasBuilder::Build();});
  _oNewButton->allowInteraction = false;

  _oLoadButton->isActive = false;
  _oLoadButton->color = buttonsColor;
  _oLoadButton->text = "Load image";
  _oLoadButton->text.center = true;
  _oLoadButton->OnClick([this](){_LoadImageFromSystem();});
  _oLoadButton->allowInteraction = false;

  _oSaveButton->isActive = false;
  _oSaveButton->color = buttonsColor;
  _oSaveButton->text = "Save image";
  _oSaveButton->text.center = true;
  _oSaveButton->allowInteraction = false;

  _oSettingsButton->isActive = false;
  _oSettingsButton->color = buttonsColor;
  _oSettingsButton->text = "Settings";
  _oSettingsButton->text.center = true;
  _oSettingsButton->allowInteraction = false;
  _oSettingsButton->OnClick([](){ SettingsBuilder::Build(); });

  _buttons.push_back(_oNewButton);
  _buttons.push_back(_oLoadButton);
  _buttons.push_back(_oSaveButton);
  _buttons.push_back(_oSettingsButton);

  for (auto button : _buttons) {
    button->zLayer = _oBackground->zLayer + 1;
  }
}

void ManagerButton::Update() {
  if (_oBackground->Clicked()) {
    _listExpanded = true;
  }
  if (!_oBackground->CursorAbove()) {
    _listExpanded = false;
  }
  _AdjustSizeAndPosition();
}
void ManagerButton::SetBackgroundColor(Color color) {  }

void ManagerButton::_AdjustSizeAndPosition() {
  //... data
  Vec2f monitorSize     = Utils::View::GetCurrentMonitorSize().CastTo<float>();
  Vec2f windowSize      = Utils::View::GetWindowSize().CastTo<float>();
  float smallerEdge          = Utils::View::GetSmallerMonitorEdge();
  float listButtonSeparation = smallerEdge * _listElementSeparationScale;
  float listButtonHeight     = smallerEdge * _listElementHeightScale;
  float listButtonWidth      = smallerEdge * _listElementWidthScale;
  float baseButtonMargin     = smallerEdge * _marginScale;
  float listWidth            = smallerEdge * _listElementWidthScale;
  float listHeight           = smallerEdge * (_buttons.size() * (_listElementHeightScale + _listElementSeparationScale) + _listElementSeparationScale);
  Vec2f listSize           = { listWidth, listHeight };
  Vec2f listPos            = { windowSize.x - listSize.x - baseButtonMargin, baseButtonMargin };

  //... base
  _oBackground->size = {monitorSize.x * _sizeScale, monitorSize.x * _sizeScale};
  _oBackground->position = {
    windowSize.x - _oBackground->size.x - baseButtonMargin,
    baseButtonMargin
  };

  //... buttons interactions
  // before others, cuz "isActive" overrides click
  // if (_uiObject_newButton->Clicked()) BlankCanvasBuilder::Build();
  // else if (_uiObject_loadButton->Clicked()) _LoadImageFromSystem();

  //... interaction handling
  Vec2f postAnimatedPosition = _oBackground->position;
  Vec2f postAnimatedSize = _oBackground->size;
  if (_listExpanded) {
    float animationTime = 0.15f;
    postAnimatedPosition  = Animator::AnimatePosition(_oBackground, listPos, animationTime, GAUSSIAN);
    postAnimatedSize      = Animator::AnimateSize(_oBackground, listSize, animationTime, GAUSSIAN);

    float sizeScale = _oBackground->size.x / postAnimatedSize.x;
    Animator::AnimateRoundness(_oBackground, _oBackground->roundness * sizeScale, animationTime, GAUSSIAN);
    Animator::AnimateImageAlpha(_oBackground, 0, animationTime, GAUSSIAN);

    for (auto button : _buttons) {
      button->isActive = true;
    }

    new DelayedAction_inSeconds(animationTime, [this]() {
      for (auto button : _buttons) {
        button->allowInteraction = true;
      }
    });
  }
  else if (_oBackground->CursorAbove()) {
    Animator::SizeUp(_oBackground);
  }
  else {
    Animator::Reset(_oBackground);
    for (auto button : _buttons) { button->isActive = false; button->allowInteraction = false; }
  }


  //... buttons
  // scaling buttons to match manager button size
  int buttonCount = _buttons.size();
  float totalHeight = postAnimatedSize.y;
  float buttonHeight = (totalHeight - (buttonCount + 1) * listButtonSeparation) / buttonCount;
  Vec2f buttonSize = {
    postAnimatedSize.x - listButtonSeparation * 2,
    buttonHeight
  };

  for (int i = 0; i < buttonCount; i++) {
    auto button = _buttons[i];
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