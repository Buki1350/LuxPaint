#include "Input.h"

#include "Math/Vec2.h"

namespace uti::input {

Vec2f getMousePosition() {
  return GetMousePosition();
}

bool mouseReleased() {
  return IsMouseButtonReleased(0);
}

bool mousePressed() {
  return IsMouseButtonPressed(0);
}

bool mouseDown() {
  return IsMouseButtonDown(0);
}

}