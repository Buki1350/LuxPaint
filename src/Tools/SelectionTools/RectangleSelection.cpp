#include "RectangleSelection.h"
#include "../../App.h"
#include "../../Canvas/Selectors/MarchingAntsSelector.h"
#include "../../Canvas/Selectors/SelectionMask.h"
#include "../../Shared/Utils/Utils.h"
#include "raylib.h"

void RectangleSelection::HandleMousePressed(UIObject*) {

    if (Utils::Input::MousePressed()) {

        _selecting = true;

        _startPos = Utils::Input::GetMousePosition();
        _endPos = _startPos;

        // Tworzymy nową ramkę selekcji
        _currentSelection = new UIObject();
        _selections.push_back(_currentSelection);

        // 1×1 obraz BLANK – nie trzeba aktualizować co frame
        Image tmp = GenImageColor(1, 1, BLANK);
        _currentSelection->SetImage(tmp);
        UnloadImage(tmp);

        _currentSelection->SetZLayer(1000);

        // Efekt "marching ants"
        MarchingAntsSelector::StartOn(_currentSelection);
    }
}

void RectangleSelection::HandleMouseDown(UIObject*) {

    if (_selecting && Utils::Input::MouseDown()) {

        _endPos = Utils::Input::GetMousePosition();
        Rectangle rect = GetSelectionRect();

        if (_currentSelection) {
            _currentSelection->position = { rect.x, rect.y };
            _currentSelection->size = { rect.width, rect.height };
        }
    }
}

void RectangleSelection::HandleMouseRelease(UIObject*) {

    if (_selecting && Utils::Input::MouseReleased()) {

        _selecting = false;

        Rectangle rect = GetSelectionRect();

        SelectionMask mask;
        mask.CreateFromRect(
            rect,
            {
                (float)App::Instance->canvas.GetImages()[0]->size.x,
                (float)App::Instance->canvas.GetImages()[0]->size.y
            }
        );

        // SHIFT = kumulacja selekcji
        if (!IsKeyDown(KEY_LEFT_SHIFT) && !IsKeyDown(KEY_RIGHT_SHIFT)) {

            for (auto* sel : _selections) {
                if (sel != _currentSelection) {
                    MarchingAntsSelector::StopOn(sel);
                    sel->Destroy();
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
