#include "Slider.h"

#include "../../StaticShared/Utils/Utils.h"

#include <algorithm>

Slider::Slider(ORIENTATION orientation) {
  _orientation = orientation;

  _oLine = new UIObject;
  _oLine->color = Utils::Files::LoadColor("sliderLine");
  _oLine->outlineScale = 0;
  _oLine->roundness = 1;
  _oGrab = new UIObject;
  _oGrab    ->color = Utils::Files::LoadColor("sliderGrab");
  _oGrab->outlineScale = 0;
  _oGrab->roundness = 1;
}

void Slider::Update() {
  float scale = Utils::View::GetSmallerMonitorEdge();
  float lineThickness = SLIDER_LINEWIDTH_SCALE * scale;
  Vec2f grabSize = Vec2f(SLIDER_GRABPOINT_SCALE * scale);

  _oLine->zLayer = this->zLayer + 1;
  _oGrab->zLayer = this->zLayer + 1;

  if (_oGrab->Pressed()) _grabbing = true;
  if (_grabbing && Utils::Input::MouseReleased()) _grabbing = false;

  switch (_orientation) {
    case HORIZONTAL: {
      _oLine->size = {this->size.x, lineThickness};
      float centeredPosY = this->position.y + (this->size.y - lineThickness) / 2;
      _oLine->position = {this->position.x, centeredPosY};

      float grabPositionX = this->position.x + this->size.x * _value;
      float grabPositionY = this->position.y + (this->size.y - grabSize.x) / 2; //grab is even on `x` and `y`
      _oGrab->size = grabSize;
      _oGrab->position = {grabPositionX, grabPositionY};

      if (_grabbing) {
        _oGrab->position.x = Utils::Input::GetMousePosition().x - _oGrab->size.x / 2;

        if (_oGrab->position.x < position.x)
          _oGrab->position.x = position.x;
        else if (_oGrab->position.x + _oGrab->size.x > position.x + size.x)
          _oGrab->position.x = position.x + size.x - _oGrab->size.x;

        // Oblicz nową wartość slidera w zakresie 0..1
        float relativePos = (_oGrab->position.x - position.x) / size.x;
        _value = std::clamp(relativePos, 0.0f, 1.0f);
      }

      break;
    }
    case VERTICAL: {

      break;
    }
  }
}

void Slider::OnValueChanged(std::function<void()> lambdaFunction) {}

float Slider::GetValue() { return _value; }
void Slider::SetValue(float value) { _value = value; }

void Slider::Destroy() {
  _oLine->Destroy();
  _oGrab->Destroy();
  UIObject::Destroy();
}