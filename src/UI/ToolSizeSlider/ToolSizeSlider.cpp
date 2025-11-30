#include "ToolSizeSlider.h"
#include "../../App.h"
#include "../../Shared/Utils/Utils.h"

#include "ToolSizeSlider.h"

void ToolSizeSlider::Init() {
    _slider = new Slider(ORIENTATION::VERTICAL);
    _inputField = new InputField();

    this->position = Vec2f(0, 25);
    this->color = Utils::Files::LoadColor("ToolSizeSlider", "uiGlobal");
    this->zLayer = 100;
    this->name = "ToolSizeSlider";

    // Reakcja slidera na zmianę wartości
    _slider->OnValueChanged([this]() {
        _inputField->text = std::to_string((int)_GetCurrentValue());
        _inputField->Update();
    });
}

Vec2f ToolSizeSlider::_CalculateShownPosition() {
    Vec2f separator = Vec2f(0, 30);
    return Calc::UIO::LeftCorner(&App::Instance->toolBox) + separator;
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

    // --- panel size ---
    this->size = Vec2f(App::Instance->toolBox.size.x, _heightScale * smallerMonitorEdge);
    this->roundness = UI_WIDGETS_ROUNDNESS * smallerMonitorEdge;

    float margin = smallerMonitorEdge * 0.01f;

    // --- input field ---
    _inputField->position = this->position + Vec2f(margin, margin);
    _inputField->size = Vec2f(size.x - 2 * margin, 25);
    _inputField->text.center = true;
    _inputField->zLayer = zLayer + 1;

    // --- slider ---
    _slider->position = position + Vec2f(0, _inputField->size.y + margin * 2);
    _slider->size = size - Vec2f(0, _inputField->size.y + margin * 3);
    _slider->zLayer = zLayer + 1;

    // --- show/hide logic ---
    static bool hovered = false;
    if (auto currentTool = App::Instance->canvas.GetCurrentTool(); currentTool != nullptr && currentTool->CanSizeBeChanged()) {
        if (!hovered) {
            _Show();
            hovered = true;
        }
    } else {
        _Hide();
        hovered = false;
    }

    // --- update slider value na aktualny rozmiar ---
    if (auto tool = App::Instance->canvas.GetCurrentTool()) {
        _slider->SetValue(tool->GetSize());
        _inputField->text = std::to_string((int)tool->GetSize());
    }
}
