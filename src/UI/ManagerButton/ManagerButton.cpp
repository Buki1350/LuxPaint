//
// Created by lukas on 31.08.2025.
//

#include "ManagerButton.h"

#include "../../../otherLibs/nfd/nfd.h"
#include "../../App/App.h"
#include "../../Shared/Animator/Animator.h"
#include "../../Shared/DelayedAction/DelayedAction_inSeconds.h"
#include "../../Shared/Serializer/Serializer.h"
#include "../MiniMenus/BlankCanvas/BlankCanvasBuilder.h"
#include "../MiniMenus/Settings/SettingsBuilder.h"
#include "Shared/Utils/Files/utiFiles.h"

void ManagerButton::init() {
  this->color = uti::files::loadColor("managerButton", "uiGlobal");
  this->setImage(Serializer::loadImage("manager_button.png"));
  this->setZLayer(LAYER_WIDGETS);
  this->imageMarginScale = UIOBJECT_ICON_MARGIN;

  _oNewButton = new Button();
  _oLoadButton = new Button();
  _oSaveButton = new Button();
  _oSettingsButton = new Button();

  Color buttonsColor = uti::files::loadColor("managerButton_listButton");

  _oNewButton->color = buttonsColor;
  _oNewButton->text = "New";
  _oNewButton->text.center = true;
  _oNewButton->onClick([](){BlankCanvasBuilder::Build();});
  _oNewButton->allowInteraction = false;

  _oLoadButton->isActive = false;
  _oLoadButton->color = buttonsColor;
  _oLoadButton->text = "Load image";
  _oLoadButton->text.center = true;
  _oLoadButton->onClick([this](){loadImageFromSystem();});
  _oLoadButton->allowInteraction = false;

  _oSaveButton->isActive = false;
  _oSaveButton->color = buttonsColor;
  _oSaveButton->text = "Save image";
  _oSaveButton->text.center = true;
  _oSaveButton->allowInteraction = false;
  _oSaveButton->onClick([this](){ saveImageToSystem(); });

  _oSettingsButton->isActive = false;
  _oSettingsButton->color = buttonsColor;
  _oSettingsButton->text = "Settings";
  _oSettingsButton->text.center = true;
  _oSettingsButton->allowInteraction = false;
  _oSettingsButton->onClick([](){ SettingsBuilder::Build(); });

  _buttons.push_back(_oNewButton);
  _buttons.push_back(_oLoadButton);
  _buttons.push_back(_oSaveButton);
  _buttons.push_back(_oSettingsButton);

  for (auto button : _buttons) {
    button->setZLayer(this->getZLayer() + 1);
  }
}

void ManagerButton::update() {
  if (this->clicked()) {
    _listExpanded = true;
  }
  if (!this->cursorAbove()) {
    _listExpanded = false;
  }
  adjustSizeAndPosition();
}

void ManagerButton::adjustSizeAndPosition() {
  //... data
  Vec2f monitorSize     = uti::view::getCurrentMonitorSize().CastTo<float>();
  Vec2f windowSize      = uti::view::getWindowSize().CastTo<float>();
  float smallerEdge          = uti::view::getSmallerMonitorEdge();
  float listButtonSeparation = smallerEdge * _listElementSeparationScale;
  float baseButtonMargin     = smallerEdge * _marginScale;
  float listWidth            = smallerEdge * _listElementWidthScale;
  float listHeight           = smallerEdge * (_buttons.size() * (_listElementHeightScale + _listElementSeparationScale) + _listElementSeparationScale);
  Vec2f listSize           = { listWidth, listHeight };
  Vec2f listPos            = { windowSize.x - listSize.x - baseButtonMargin, baseButtonMargin };

  this->roundness = UI_WIDGETS_ROUNDNESS * uti::view::getSmallerMonitorEdge();

  //... base
  this->size = {monitorSize.x * _sizeScale, monitorSize.x * _sizeScale};
  this->position = {
    windowSize.x - this->size.x - baseButtonMargin,
    baseButtonMargin
  };

  //... interaction handling
  Vec2f postAnimatedPosition = this->position;
  Vec2f postAnimatedSize = this->size;
  if (_listExpanded) {
    float animationTime = ANIMATION_POPUP_DURATION;
    postAnimatedPosition  = Animator::animatePosition(this, listPos, animationTime, GAUSSIAN);
    postAnimatedSize      = Animator::animateSize(this, listSize, animationTime, GAUSSIAN);

    float sizeScale = this->size.x / postAnimatedSize.x;
    Animator::animateRoundness(this, this->roundness * sizeScale, animationTime, GAUSSIAN);
    Animator::animateImageAlpha(this, 0, animationTime, GAUSSIAN);

    for (auto button : _buttons) {
      button->isActive = true;
    }

    new DelayedAction_inSeconds(animationTime, [this]() {
      for (auto button : _buttons) {
        button->allowInteraction = true;
      }
    });
  }
  else if (this->cursorAbove()) {
    Animator::sizeUp(this);
  }
  else {
    Animator::reset(this);
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

void ManagerButton::loadImageFromSystem() {
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
    App::instance().canvas.addImage(img);
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

void ManagerButton::saveImageToSystem() {
  char* outPath = nullptr;

  nfdu8filteritem_t filters[2] = {
    { "PNG Image", "png" },
    { "JPG Image", "jpg,jpeg" }
  };

  nfdresult_t result = NFD_SaveDialog(
          &outPath,
          filters,
          2,
          nullptr,
          "canvas.png"
  );

  if (result == NFD_OKAY) {

    Image img = App::instance().canvas.exportAsImage();

    // raylib sam rozpoznaje format po rozszerzeniu
    ExportImage(img, outPath);

    UnloadImage(img);
    NFD_FreePath(outPath);
  }
  else if (result == NFD_CANCEL) {
    // anulowano
  }
  else {
    printf("NFD Error: %s\n", NFD_GetError());
  }
}
