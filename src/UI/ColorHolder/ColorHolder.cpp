//
// Created by lukas on 5.09.2025.
//

#include "ColorHolder.h"

#include "../../Render/UIObjectsManager.h"
#include "../../StaticShared/Utils/Utils.h"
#include "../ColorPicker/ColorPicker.h"

void ColorHolder::Init() {
  _oBackground = new UIObject();
  _oBackground->color = Utils::LoadColor("colorHolderButton");
  _oBackground->roundness = 0.3f;
  _oBackground->zLayer = 1;

  Button* colorPickButton = new Button();
  colorPickButton->color = WHITE;
  colorPickButton->zLayer = _oBackground->zLayer + 1;
  colorPickButton->roundness = _oBackground->roundness;
  colorPickButton->OnClick([this]() {
    _CreateColorPicker();
  });
  _buttons.push_back(colorPickButton);
}

void ColorHolder::Update() {
  float screenScale = Utils::GetSmallerMonitorEdge();
  float windowHeight = Utils::GetWindowSize().y;
  float separatorSize = _separatorScale * screenScale;
  float offset = _offsetScale * screenScale;
  Vec2f buttonSize = Vec2f(_buttonScale * screenScale);

  _oBackground->size = Vec2f(
    separatorSize + _buttons.size() * (buttonSize.x + separatorSize),
    buttonSize.y + separatorSize * 2
    );
  _oBackground->position = Vec2f(offset, windowHeight - separatorSize - _oBackground->size.y);

  for (int i = 0; i < _buttons.size(); i++) {
    _buttons[i]->size = buttonSize;
    _buttons[i]->position = Vec2f(
      _oBackground->position.x + separatorSize + (buttonSize.x + separatorSize) * i,
      _oBackground->position.y + separatorSize
      );
  }
}

void ColorHolder::_CreateColorPicker() {
  float screenScale = Utils::GetSmallerMonitorEdge();
  float windowHeight = Utils::GetWindowSize().y;
  float separatorSize = _separatorScale * screenScale;
  float offset = _offsetScale * screenScale;
  Vec2f buttonSize = Vec2f(_buttonScale * screenScale);


  ColorPicker* cp = new ColorPicker();
  cp->size = Vec2f(300, 300);
  cp->position = Vec2f(
    separatorSize,
    _oBackground->position.y + separatorSize + cp->size.y
    );
  cp->color = WHITE;
  cp->zLayer = 5;
}