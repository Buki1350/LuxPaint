//
// Created by lukas on 5.09.2025.
//

#include "ColorHolder.h"

#include "../../App.h"
#include "../../Shared/Utils/Utils.h"
#include "../../Shared/UIObjects/UIObjectsManager.h"
#include "../ColorPicker/ColorPicker.h"

void ColorHolder::Init() {
  this->color = Utils::Files::LoadColor("colorHolder", "uiGlobal");
  this->SetZLayer(LAYER_WIDGETS);

  Button* colorPickButton = new Button();
  colorPickButton->color = WHITE;
  colorPickButton->SetZLayer(this->GetZLayer() + 1);
  colorPickButton->roundness = this->roundness;
  colorPickButton->SetImage(FilesManager::LoadImage("rgb.png"));
  colorPickButton->imageMarginScale = UIOBJECT_ICON_MARGIN;
  colorPickButton->OnClick([this]() {
    _CreateColorPicker();
  });
  _buttons.push_back(colorPickButton);

  for (int i = 0; i < _numberOfSavedColors; i++) {
    Button* savedColor = new Button();
    savedColor->color = WHITE;
    savedColor->SetZLayer(this->GetZLayer() + 1);
    savedColor->roundness = 1;
    savedColor->OnClick([this, savedColor]() {
      App::Instance->canvas.SetCurrentColor(savedColor->color);
    });
    _buttons.push_back(savedColor);
  }
}

void ColorHolder::Update() {
  float screenScale = Utils::View::GetSmallerMonitorEdge();
  float windowHeight = Utils::View::GetWindowSize().y;
  float separatorSize = _separatorScale * screenScale;
  float offset = _offsetScale * screenScale;
  Vec2f buttonSize = Vec2f(_buttonScale * screenScale);

  this->roundness = UI_WIDGETS_ROUNDNESS * Utils::View::GetSmallerMonitorEdge();

  this->size = Vec2f(
    separatorSize + _buttons.size() * (buttonSize.x + separatorSize),
    buttonSize.y + separatorSize * 2
    );
  this->position = Vec2f(offset, windowHeight - separatorSize - this->size.y);

  for (int i = 0; i < _buttons.size(); i++) {
    _buttons[i]->size = buttonSize;
    _buttons[i]->position = Vec2f(
      this->position.x + separatorSize + (buttonSize.x + separatorSize) * i,
      this->position.y + separatorSize
      );
  }
}

void ColorHolder::_CreateColorPicker() {
  float screenScale = Utils::View::GetSmallerMonitorEdge();
  float separatorSize = _separatorScale * screenScale;

  ColorPicker *cp = new ColorPicker();
  cp->position = Vec2f(separatorSize,
                       this->position.y - separatorSize - cp->size.y);

  cp->SetColor(App::Instance->canvas.GetCurrentColor());

  cp->OnColorChange([this](Color newColor) {
    if (!_buttons.empty()) {
      _buttons[0]->color = newColor;
    }
    App::Instance->canvas.SetCurrentColor(newColor);
  });
}

void ColorHolder::SetBackgroundColor(Color color) { this->color = color; }