#include "RectangleSelection.h"
#include "../../App/App.h"
#include "../../Canvas/Selectors/MarchingAntsSelector.h"
#include "../../Canvas/Selectors/SelectionMask.h"
#include "raylib.h"

void RectangleSelection::_handleMousePressedImpl(UIObject*) {

    if (uti::input::mousePressed()) {

        _selecting = true;

        _startPos = uti::input::getMousePosition();
        _endPos = _startPos;

        // Tworzymy nową ramkę selekcji
        _currentSelection = new UIObject();
        _selections.push_back(_currentSelection);

        // 1×1 obraz BLANK – nie trzeba aktualizować co frame
        Image tmp = GenImageColor(1, 1, BLANK);
        _currentSelection->setImage(tmp);
        UnloadImage(tmp);

        _currentSelection->setZLayer(1000);

        // Efekt "marching ants"
        MarchingAntsSelector::startOn(_currentSelection);
    }
}

void RectangleSelection::_handleMouseDownImpl(UIObject*) {

    if (_selecting && uti::input::mouseDown()) {

        _endPos = uti::input::getMousePosition();
        Rectangle rect = GetSelectionRect();

        if (_currentSelection) {
            _currentSelection->position = { rect.x, rect.y };
            _currentSelection->size = { rect.width, rect.height };
        }
    }
}

void RectangleSelection::_handleMouseReleaseImpl(UIObject*) {

    if (_selecting && uti::input::mouseReleased()) {

        _selecting = false;

        Rectangle rect = GetSelectionRect();

        SelectionMask mask;
        mask.CreateFromRect(
            rect,
            {
                (float)App::instance->canvas.getLayersInfo()[0]->size.x,
                (float)App::instance->canvas.getLayersInfo()[0]->size.y
            }
        );

        // SHIFT = kumulacja selekcji
        if (!IsKeyDown(KEY_LEFT_SHIFT) && !IsKeyDown(KEY_RIGHT_SHIFT)) {

            for (auto* sel : _selections) {
                if (sel != _currentSelection) {
                    MarchingAntsSelector::stopOn(sel);
                    sel->destroy();
                }
            }

            _selections.clear();
            _selections.push_back(_currentSelection);
        }

        _currentSelection = nullptr;
    }
}

Rectangle RectangleSelection::GetSelectionRect() const {

    Rectangle rect;
    rect.x = fmin(_startPos.x, _endPos.x);
    rect.y = fmin(_startPos.y, _endPos.y);
    rect.width = fabs(_endPos.x - _startPos.x);
    rect.height = fabs(_endPos.y - _startPos.y);
    return rect;
}
