#include "Utils.h"
#include "../FilesManager/FilesManager.h"
#include <raylib.h>

static Utils _; // singleton declaration

Vec2i Utils::_monitorSize;
Vec2i Utils::_windowSize;
float Utils::_smallerMonitorEdge;
Font Utils::_defaultFont;
Vec2f Utils::_mousePosition;
float Utils::_deltaTime;

Vec2i Utils::_GetCurrentMonitorSize() {
  int monitor = GetCurrentMonitor();
  int monitorHeight = (int)GetMonitorHeight(monitor);
  int monitorWidth = (int)GetMonitorWidth(monitor);
  return {monitorWidth, monitorHeight};
}

Vec2i Utils::_GetWindowSize() {
  return {GetScreenWidth(), GetScreenHeight()};
}

float Utils::_GetSmallerMonitorEdge() {
  return _monitorSize.y < _monitorSize.x? _monitorSize.y : _monitorSize.x;
}

void Utils::Update() {
  _monitorSize = Utils::_GetCurrentMonitorSize();
  _windowSize = Utils::_GetWindowSize();
  _smallerMonitorEdge = Utils::_GetSmallerMonitorEdge();
  Vector2 RLmousePosition = ::GetMousePosition();
  _mousePosition = {RLmousePosition.x, RLmousePosition.y};
  _deltaTime = GetFrameTime();
}

bool Utils::RaylibSpecific::VectorsEqual(Vector2 start, Vector2 end) {
  return start.x == end.x && start.y == end.y;
}

