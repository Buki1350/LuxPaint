#include "ToolSizeSlider.h"
#include "../../App.h"
#include "../../Shared/Utils/Utils.h"

#include "ToolSizeSlider.h"

#include "../../Shared/DelayedAction/DelayedAction_until.h"

#include <iostream>

void ToolSizeSlider::Init() {
    _inputField = new InputField();
    _inputField->text.center = true;
    _inputField->SetMode(InputMode::NUMBERS_ONLY);
    _inputField->OnValueChanged(
        [this] {
        _UpdateSliderFromInput();
    });

    _slider = new Slider(ORIENTATION::VERTICAL);
    _slider->OnValueChanged([this] {
        _UpdateInputFromSlider(_slider->GetValue());
    });
    _slider->OnRelease([this] {
        std::string valueS = _inputField->GetValue();
        int value = std::stoi(valueS.empty()? "1" : valueS);
        minVal = value / 5;
        maxVal = value * 5;
    });

    new DelayedAction_until(
        [this] { return _currentTool != nullptr; },
        [this] {
            std::string valueS = _inputField->GetValue();
            int value = std::stoi(valueS.empty()? "1" : valueS);
            minVal = value / 5;
            maxVal = value * 5;
        });

    this->position = Vec2f(0, 25);
    this->color = Utils::Files::LoadColor("ToolSizeSlider", "uiGlobal");
    this->zLayer = 100;
    this->name = "ToolSizeSlider";
}

Vec2f ToolSizeSlider::_CalculateShownPosition() {
  Vec2f separator = Vec2f(0, 30);
  return Calc::UIO::LeftCorner(&App::Instance->toolBox) + separator;
}

void ToolSizeSlider::_UpdateSliderFromInput() {
    if (!_currentTool) return;

    std::string valStr = _inputField->GetValue();
    if (valStr.empty()) return;

    int inputVal = std::stoi(valStr);
    int newVal = inputVal;

    float sliderValue = (float)(newVal - minVal) / (maxVal - minVal);
    _slider->SetValue(sliderValue);

    _currentTool->SetSize(newVal);
}


void ToolSizeSlider::_UpdateInputFromSlider(float sliderValue) {
    if (!_currentTool) return;

    int newVal = std::round(sliderValue * 100) / 100 * maxVal / 2;
    newVal = std::max(newVal, 1);
    _inputField->SetValue(std::to_string(newVal));
    _currentTool->SetSize(newVal);
    _slider->SetValue(.5f);
    //std::cout << sliderValue << std::endl;
}


Vec2f ToolSizeSlider::_CalculateHiddenPosition() {
    Vec2f separator = Vec2f(0, 30);
    Vec2f newPosition = Calc::UIO::LeftCorner(&App::Instance->toolBox) + separator;
    newPosition.x = -size.x - Utils::View::GetSmallerMonitorEdge() * UIOBJECT_OUTLINE_SCALE;
    return newPosition;
}

void ToolSizeSlider::_Show() {
    float showTime = ANIMATION_SIZEUP_DURATION;
    Animator::AnimatePosition(this, _CalculateShownPosition(), showTime, GAUSSIAN);
}

void ToolSizeSlider::_Hide() {
    float hideTime = ANIMATION_SIZEUP_DURATION;
    Animator::AnimatePosition(this, _CalculateHiddenPosition(), hideTime, GAUSSIAN);
}

float ToolSizeSlider::_GetCurrentValue() {
    return _maxSliderValue / 2;
}

void ToolSizeSlider::Update() {
    float smallerMonitorEdge = Utils::View::GetSmallerMonitorEdge();
    _currentTool = App::Instance->canvas.GetCurrentTool();

    // --- panel size ---
    this->size = Vec2f(App::Instance->toolBox.size.x, _heightScale * smallerMonitorEdge);
    this->roundness = UI_WIDGETS_ROUNDNESS * smallerMonitorEdge;

    float margin = smallerMonitorEdge * 0.01f;

    // --- input field ---
    _inputField->position = this->position + Vec2f(margin, margin);
    _inputField->size = Vec2f(size.x - 2 * margin, 25);
    _inputField->zLayer = zLayer + 1;

    // --- slider and values ---
    if ((minVal == -1 || maxVal == -1) && _currentTool != nullptr) {
        float sliderVal = _slider->GetValue();
        if ((minVal == -1 || maxVal == -1) && sliderVal != 0) {
            maxVal = sliderVal * 5;
            minVal = sliderVal / 5;
        }

        _inputField->SetValue("1");
        _UpdateSliderFromInput();
    }

    _slider->position = position + Vec2f(0, _inputField->size.y + margin * 2);
    _slider->size = size - Vec2f(0, _inputField->size.y + margin * 3);
    _slider->zLayer = zLayer + 1;


    // --- show/hide logic ---
    static bool hovered = false;
    if (_currentTool != nullptr && _currentTool->CanSizeBeChanged()) {
        if (!hovered) {
            _Show();
            hovered = true;
        }
    } else {
        _Hide();
        hovered = false;
    }
}
