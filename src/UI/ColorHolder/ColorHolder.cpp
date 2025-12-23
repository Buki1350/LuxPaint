//
// Created by lukas on 5.09.2025.
//

#include "ColorHolder.h"

#include "../../App/App.h"
#include "../../Shared/UIObjects/UIObjectsManager.h"
#include "../ColorPicker/ColorPicker.h"
#include "Shared/Utils/Files/utiFiles.h"
#include "Shared/Utils/View/utiView.h"

void ColorHolder::init() {
  this->color = uti::files::loadColor("colorHolder", "uiGlobal");
  this->setZLayer(LAYER_WIDGETS);

  Button* colorPickButton = new Button();
  colorPickButton->color = WHITE;
  colorPickButton->setZLayer(this->getZLayer() + 1);
  colorPickButton->roundness = this->roundness;
  colorPickButton->setImage(Serializer::LoadImage("rgb.png"));
  colorPickButton->imageMarginScale = UIOBJECT_ICON_MARGIN;
  colorPickButton->onClick([this]() {
    _createColorPicker();
  });
  _buttons.push_back(colorPickButton);

  for (int i = 0; i < _numberOfSavedColors; i++) {
    Button* savedColor = new Button();
    savedColor->color = WHITE;
    savedColor->setZLayer(this->getZLayer() + 1);
    savedColor->roundness = 1;
    savedColor->onClick([this, savedColor]() {
      App::instance->canvas.setCurrentColor(savedColor->color);
    });
    _buttons.push_back(savedColor);
  }
}

void ColorHolder::update() {
  float screenScale = uti::view::getSmallerMonitorEdge();
  float windowHeight = uti::view::getWindowSize().y;
  float separatorSize = _separatorScale * screenScale;
  float offset = _offsetScale * screenScale;
  Vec2f buttonSize = Vec2f(_buttonScale * screenScale);

  this->roundness = UI_WIDGETS_ROUNDNESS * uti::view::getSmallerMonitorEdge();

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

void ColorHolder::_createColorPicker() {
  float screenScale = uti::view::getSmallerMonitorEdge();
  float separatorSize = _separatorScale * screenScale;

  ColorPicker *cp = new ColorPicker();
  cp->position = Vec2f(separatorSize,
                       this->position.y - separatorSize - cp->size.y);

  cp->setColor(App::instance->canvas.getCurrentColor());

  cp->onColorChange([this](Color newColor) {
    if (!_buttons.empty()) {
      _buttons[0]->color = newColor;
    }
    App::instance->canvas.setCurrentColor(newColor);
  });
}

void ColorHolder::setBackgroundColor(Color color) { this->color = color; }