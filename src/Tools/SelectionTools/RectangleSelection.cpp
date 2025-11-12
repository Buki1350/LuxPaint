#include "RectangleSelection.h"
#include "raylib.h"
#include "../../Render/UIObjectsManager.h"
#include "../../Canvas/Selectors/MarchingAntsSelector.h"
#include "../../Canvas/Selectors/SelectionMask.h"

void RectangleSelection::_HandleMousePressed(UIObject*, Vec2f) {
    if (Utils::Input::MousePressed()) {
        _selecting = true;
        _startPos = Utils::Input::GetMousePosition();
        _endPos = _startPos;

        // Tworzymy nową selekcję tylko na początku
        _currentSelection = new UIObject();
        _selections.push_back(_currentSelection);

        int w = 1, h = 1; // tymczasowe, potem będziemy aktualizować w MouseDown
        Image tmp = GenImageColor(w, h, BLANK);
        _currentSelection->SetImage(tmp);
        UnloadImage(tmp);

        _currentSelection->zLayer = 1000;

        // Dodajemy animowane mrówki
        MarchingAntsSelector::StartOn(_currentSelection);
    }
}

void RectangleSelection::_HandleMouseDown(UIObject*, Vec2f) {
    if (_selecting && Utils::Input::MouseDown()) {
        _endPos = Utils::Input::GetMousePosition();
        Rectangle rect = GetSelectionRect();

        if (_currentSelection) {
            _currentSelection->position = { rect.x, rect.y };
            _currentSelection->size = { rect.width, rect.height };

            // Aktualizacja tekstury UIObject
            Image tmp = GenImageColor((int)ceilf(rect.width), (int)ceilf(rect.height), BLANK);
            _currentSelection->SetImage(tmp);
            UnloadImage(tmp);
        }
    }
}

void RectangleSelection::_HandleMouseRelease(UIObject*, Vec2f) {
    if (_selecting && Utils::Input::MouseReleased()) {
        _selecting = false;

        Rectangle rect = GetSelectionRect();

        SelectionMask mask;
        mask.CreateFromRect(rect, {
            (float)App::Instance->canvas.GetImages()[0]->size.x,
            (float)App::Instance->canvas.GetImages()[0]->size.y
        });

        // Jeśli SHIFT nie jest wciśnięty, zatrzymujemy poprzednie zaznaczenia
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
