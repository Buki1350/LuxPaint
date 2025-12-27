#include "ToolSizeSlider.h"
#include "../../App/App.h"
#include "../../Shared/DelayedAction/DelayedAction_until.h"
#include "Shared/Utils/Files/utiFiles.h"
#include "Shared/Utils/View/utiView.h"

#include <iostream>

void ToolSizeSlider::init() {
    _inputField = new InputField();
    _inputField->text.center = true;
    _inputField->setMode(InputMode::NUMBERS_ONLY);
    _inputField->onValueChanged(
        [this] {
        _updateSliderFromInput();
    });

    _slider = std::make_unique<Slider>(ORIENTATION::VERTICAL);
    _slider->onValueChanged([this] {
        _updateInputFromSlider(_slider->getValue());
    });
    _slider->onRelease([this] {
        std::string valueS = _inputField->getValue();
        int value = std::stoi(valueS.empty()? "1" : valueS);
        _minVal = value / 5;
        _maxVal = value * 5;
    });

    new DelayedAction_until(
        [this] { return _currentTool != nullptr; },
        [this] {
            std::string valueS = _inputField->getValue();
            int value = std::stoi(valueS.empty()? "1" : valueS);
            _minVal = value / 5;
            _maxVal = value * 5;
        });

    this->position = Vec2f(0, 25);
    this->color = uti::files::loadColor("ToolSizeSlider", "uiGlobal");
    this->setZLayer(LAYER_WIDGETS);
    this->name = "ToolSizeSlider";
}

Vec2f ToolSizeSlider::_calculateShownPosition() {
  Vec2f separator = Vec2f(0, 30);
  return Calc::UIO::LeftCorner(&App::instance->toolBox) + separator;
}

void ToolSizeSlider::_updateSliderFromInput() {
    if (!_currentTool) return;

    std::string valStr = _inputField->getValue();
    if (valStr.empty()) return;

    int inputVal = std::stoi(valStr);
    int newVal = inputVal;

    float sliderValue = (float)(newVal - _minVal) / (_maxVal - _minVal);
    _slider->setValue(sliderValue);

    _currentTool->setSize((float)newVal);
}


void ToolSizeSlider::_updateInputFromSlider(float sliderValue) {
    if (!_currentTool) return;

    int newVal = (int)(std::round(sliderValue * 100) / 100 * _maxVal / 2);
    newVal = std::max(newVal, 1);
    _inputField->setValue(std::to_string(newVal));
    _currentTool->setSize((float)newVal);
    _slider->setValue(.5f);
    //std::cout << sliderValue << std::endl;
}


Vec2f ToolSizeSlider::_calculateHiddenPosition() {
    Vec2f separator = Vec2f(0, 30);
    Vec2f newPosition = Calc::UIO::LeftCorner(&App::instance->toolBox) + separator;
    newPosition.x = -size.x - uti::view::getSmallerMonitorEdge() * UIOBJECT_OUTLINE_SCALE;
    return newPosition;
}

void ToolSizeSlider::_show() {
    float showTime = ANIMATION_SIZEUP_DURATION;
    Animator::animatePosition(this, _calculateShownPosition(), showTime, GAUSSIAN);
}

void ToolSizeSlider::_hide() {
    float hideTime = ANIMATION_SIZEUP_DURATION;
    Animator::animatePosition(this, _calculateHiddenPosition(), hideTime, GAUSSIAN);
}

float ToolSizeSlider::_getCurrentValue() {
    return _maxSliderValue / 2;
}

void ToolSizeSlider::update() {
    float smallerMonitorEdge = uti::view::getSmallerMonitorEdge();
    _currentTool = App::instance->canvas.getCurrentTool();
    _floatValue = _slider->getValue() * _maxVal;

    // --- panel size ---
    this->size = Vec2f(App::instance->toolBox.size.x, _heightScale * smallerMonitorEdge);
    this->roundness = UI_WIDGETS_ROUNDNESS * smallerMonitorEdge;
    float margin = smallerMonitorEdge * 0.01f;

    // --- input field ---
    _inputField->position = this->position + Vec2f(margin, margin);
    _inputField->size = Vec2f(size.x - 2 * margin, 25);
    _inputField->setZLayer(getZLayer() + 1);

    // --- slider and values ---
    if ((_minVal == -1 || _maxVal == -1) && _currentTool != nullptr) {
        float sliderVal = _slider->getValue();
        if ((_minVal == -1 || _maxVal == -1) && sliderVal != 0) {
            _maxVal = (int)sliderVal * 5;
            _minVal = (int)sliderVal / 5;
        }

        if (_inputField->text.value.empty())
            _inputField->setValue("1");
        _updateSliderFromInput();
    }

    _slider->position = position + Vec2f(0, _inputField->size.y + margin * 2);
    _slider->size = size - Vec2f(0, _inputField->size.y + margin * 3);
    _slider->setZLayer(getZLayer() + 1);

    // --- show/hide logic ---
    static bool hovered = false;
    if (_currentTool != nullptr && _currentTool->canSizeBeChanged()) {
        if (!hovered) {
            _show();
            hovered = true;
        }
    } else {
        _hide();
        hovered = false;
    }
}
