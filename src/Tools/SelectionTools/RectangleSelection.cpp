#include "RectangleSelection.h"
#include "raylib.h"

void RectangleSelection::HandleClick(UIObject* /*imageToPaint*/, Vec2f /*localPos*/) {
  // zaczynamy zaznaczanie przy naciśnięciu LPM
  if (Utils::MousePressed()) {
    selecting = true;
    startPos = Utils::GetMousePosition();
    endPos = startPos;
  }

  // podczas trzymania - aktualizujemy endPos i rysujemy ramkę
  if (selecting && Utils::MouseDown()) {
    endPos = Utils::GetMousePosition();

    Rectangle rect = GetSelectionRect();
    // rysujemy ramkę (wizualizacja podczas przeciągania)
    DrawRectangleLinesEx(rect, 1.5f, SKYBLUE);
  }
}

void RectangleSelection::HandleRelease(UIObject* /*imageToPaint*/, Vec2f /*localPos*/) {
  // kończymy zaznaczenie przy puszczeniu LPM
  if (selecting && Utils::MouseReleased()) {
    selecting = false;
    Rectangle rect = GetSelectionRect();
    // przekazujemy zaznaczenie do Canvas (Canvas utworzy overlay / marching ants)
    App::Instance->canvas.SetSelection(rect);
  }
}

Rectangle RectangleSelection::GetSelectionRect() const {
  Rectangle rect;
  rect.x = fmin(startPos.x, endPos.x);
  rect.y = fmin(startPos.y, endPos.y);
  rect.width = fabs(endPos.x - startPos.x);
  rect.height = fabs(endPos.y - startPos.y);
  return rect;
}
