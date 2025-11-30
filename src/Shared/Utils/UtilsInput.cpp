#pragma once
#include "Utils.h"

Vec2f Utils::Input::GetMousePosition() { return _mousePosition; }

bool Utils::Input::MouseReleased() { return IsMouseButtonReleased(0); }

bool Utils::Input::MousePressed() { return IsMouseButtonPressed(0); }

bool Utils::Input::MouseDown() { return IsMouseButtonDown(0); }