//
// Created by lukas on 30.08.2025.
//

#include "Utils.h"

#include "../FilesManager/FilesManager.h"

Vector2Int Utils::_monitorSize;
Vector2Int Utils::_windowSize;
float Utils::_smallerMonitorEdge;
Font Utils::_defaultFont;

Vector2Int Utils::GetCurrentMonitorSize() { return _monitorSize; }
Vector2Int Utils::GetWindowSize() { return _windowSize; }
float Utils::GetSmallerMonitorEdge() { return _smallerMonitorEdge; }

void Utils::Init() {
  _defaultFont = LoadFont("resources/fonts/Comfortaa-Bold.ttf");
}

Color Utils::HexToColor(std::string hex) {
  unsigned int r = 0, g = 0, b = 0, a = 255;

  if (hex.size() == 7) { // #RRGGBB
    sscanf(hex.c_str(), "#%02x%02x%02x", &r, &g, &b);
  } else if (hex.size() == 9) { // #RRGGBBAA
    sscanf(hex.c_str(), "#%02x%02x%02x%02x", &r, &g, &b, &a);
  }

  return Color{
    (unsigned char)r,
    (unsigned char)g,
    (unsigned char)b,
    (unsigned char)a
  };
}

Vector2Int Utils::_GetCurrentMonitorSize() {
  int monitor = GetCurrentMonitor();
  int monitorHeight = (int)GetMonitorHeight(monitor);
  int monitorWidth = (int)GetMonitorWidth(monitor);
  return {monitorWidth, monitorHeight};
}
Vector2Int Utils::_GetWindowSize() {
  return {GetScreenWidth(), GetScreenHeight()};
}

Color Utils::LoadColor(std::string token) {
  return HexToColor(FilesManager::Load<std::string>(FILE_COLOR_PALETTE, token));
}

Font Utils::GetDefaultFont() { return _defaultFont; }

float Utils::_GetSmallerMonitorEdge() {
  return _monitorSize.y < _monitorSize.x? _monitorSize.y : _monitorSize.x;
}

void Utils::Update() {
  _monitorSize = Utils::_GetCurrentMonitorSize();
  _windowSize = Utils::_GetWindowSize();
  _smallerMonitorEdge = Utils::_GetSmallerMonitorEdge();
}
bool Utils::VectorsEqual(Vector2 start, Vector2 end) {
  return start.x == end.x && start.y == end.y;
}

