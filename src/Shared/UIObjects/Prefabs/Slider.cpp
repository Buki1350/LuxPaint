#include "Slider.h"
#include "Shared/Utils/Files/utiFiles.h"
#include "Shared/Utils/Input/Input.h"
#include "Shared/Utils/View/utiView.h"
#include <algorithm>

Slider::Slider(ORIENTATION orientation) {
  _orientation = orientation;

  _oLine = new UIObject;
  _oLine->color = uti::files::loadColor("sliderLine");
  _oLine->outlineScale = 0;
  _oLine->roundness = 1;

  _oGrab = new UIObject;
  _oGrab->color = uti::files::loadColor("sliderGrab");
  _oGrab->outlineScale = 0;
  _oGrab->roundness = 1;
}

void Slider::update() {
    float scale = uti::view::getSmallerMonitorEdge();
    float lineThickness = SLIDER_LINEWIDTH_SCALE * scale;
    Vec2f grabSize = Vec2f(SLIDER_GRABPOINT_SCALE * scale);

    _oLine->setZLayer(this->getZLayer() + 1);
    _oGrab->setZLayer(this->getZLayer() + 1);

    Vec2f globalPos = this->position;

    if (_oGrab->pressed()) _grabbing = true;
    if (_grabbing && uti::input::mouseReleased()) {
        if (_onReleaseFunc != nullptr) _onReleaseFunc();
        _grabbing = false;
    }

    if (std::isnan(_value)) _value = 0.0f;

    switch (_orientation) {
        case HORIZONTAL: {
            _oLine->size = { this->size.x, lineThickness };
            float centeredY = globalPos.y + (this->size.y - lineThickness) / 2;
            _oLine->position = { globalPos.x, centeredY };

            float grabX = globalPos.x + this->size.x * _value;
            float grabY = globalPos.y + (this->size.y - grabSize.y) / 2;

            _oGrab->size = grabSize;
            _oGrab->position = { grabX, grabY };

            if (_grabbing) {
                float mouseX = uti::input::getMousePosition().x - _oGrab->size.x / 2;
                _oGrab->position.x = mouseX;

                if (_oGrab->position.x < globalPos.x)
                    _oGrab->position.x = globalPos.x;
                else if (_oGrab->position.x + _oGrab->size.x > globalPos.x + size.x)
                    _oGrab->position.x = globalPos.x + size.x - _oGrab->size.x;

                float relative = (_oGrab->position.x - globalPos.x) / size.x;
                _value = std::clamp(relative, 0.0f, 1.0f);
            }
            break;
        }

        case VERTICAL: {
            _oLine->size = { lineThickness, this->size.y };
            float centeredX = globalPos.x + (this->size.x - lineThickness) / 2;
            _oLine->position = { centeredX, globalPos.y };

            float grabY = globalPos.y + this->size.y * (1.0f - _value);
            float grabX = globalPos.x + (this->size.x - grabSize.x) / 2;

            _oGrab->size = grabSize;
            _oGrab->position = Vec2f(grabX, grabY - grabSize.y / 2 );

            if (_grabbing) {
                float mouseY = uti::input::getMousePosition().y - grabSize.y / 2;
                _oGrab->position.y = mouseY;

                if (_oGrab->position.y < globalPos.y)
                    _oGrab->position.y = globalPos.y;
                else if (_oGrab->position.y + grabSize.y > globalPos.y + size.y)
                    _oGrab->position.y = globalPos.y + size.y - grabSize.y;

                float relative = (_oGrab->position.y - globalPos.y) / size.y;
                _value = std::clamp(1.0f - relative, 0.0f, 1.0f);
            }
            break;
        }
    }

    static float previousValue = 0.0f;
    if (previousValue != _value) {
        if (_onChangeFunc != nullptr) _onChangeFunc();
        previousValue = _value;
    }
}

void Slider::onValueChanged(std::function<void()> lambdaFunction) {
    _onChangeFunc = lambdaFunction;
}

float Slider::getValue() { return _value; }

void Slider::setValue(float value) { _value = value; }

void Slider::onRelease(std::function<void()> lambdaFunction) {
    _onReleaseFunc = lambdaFunction;
}

void Slider::destroy() {
    Updatable::markedForDeletion = true;
  _oLine->destroy();
  _oGrab->destroy();
  UIObject::destroy();
}